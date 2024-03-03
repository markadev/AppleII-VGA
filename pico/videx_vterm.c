#include "videx_vterm.h"

#include <pico/stdlib.h>
#include "buffers.h"
#include "colors.h"
#include "textfont/textfont.h"
#include "vga.h"


volatile bool videx_vterm_80col_enabled;  // true -> the annunciator to enable 80-columns is on
volatile bool videx_vterm_mem_selected;   // true -> videx memory is accessible at $C800-$CFFF
volatile uint8_t videx_vram[2048];

static uint8_t videx_banknum;   // selected memory bank
static uint8_t videx_crtc_idx;  // selected CRTC register

// CRT controller registers (Ref. MC6845 datasheet):
//   register  r/w     name
//     00       w      Horiz. Total
//     01       w      Horiz. Displayed
//     02       w      Horiz. Sync Position
//     03       w      Horiz. Sync Width
//     04       w      Vert. Total
//     05       w      Vert. Total Adjust
//     06       w      Vert. Displayed
//     07       w      Vert. Sync Position
//     08       w      Interlace Mode
//     09       w      Max Scan Line Address
//     10       w      Cursor Start
//     11       w      Cursor End
//     12       w      Start Address (H)
//     13       w      Start Address (L)
//     14      r/w     Cursor (H)
//     15      r/w     Cursor (L)
//     16       r      Light Pen (H)
//     17       r      Light Pen (L)
static volatile uint8_t videx_crtc_regs[17];

static uint_fast32_t videx_cursor_mask = 0;
static uint64_t videx_next_flash_tick = 0;


#define CURSOR_LINE_START (videx_crtc_regs[10] & 0xf)
#define CURSOR_LINE_END (videx_crtc_regs[11] & 0xf)


void videx_vterm_init() {
    // initializing registers
    videx_banknum = 0;
    videx_crtc_idx = 0;
    videx_crtc_regs[0] = 0x7b;
    videx_crtc_regs[1] = 0x50;
    videx_crtc_regs[2] = 0x62;
    videx_crtc_regs[3] = 0x29;
    videx_crtc_regs[4] = 0x1b;
    videx_crtc_regs[5] = 0x08;
    videx_crtc_regs[6] = 0x18;
    videx_crtc_regs[7] = 0x19;
    videx_crtc_regs[8] = 0x00;
    videx_crtc_regs[9] = 0x08;
    videx_crtc_regs[10] = 0xc0;
    videx_crtc_regs[11] = 0x08;
    videx_crtc_regs[12] = 0x00;
    videx_crtc_regs[13] = 0x00;
    videx_crtc_regs[14] = 0x00;
    videx_crtc_regs[15] = 0x00;
}


// Shadow accesses to card registers in $C0n0 - $C0nF range
void videx_vterm_shadow_register(bool is_write, uint_fast16_t address, uint_fast8_t data) {
    // select the video memory bank
    videx_banknum = (address & 0x000c) >> 2;

    if(!is_write)
        return;

    if(address & 0x0001) {
        // set current register value
        if(videx_crtc_idx < sizeof(videx_crtc_regs)) {
            videx_crtc_regs[videx_crtc_idx] = data;
        }
    } else {
        // set the crtc register being accessed
        videx_crtc_idx = data;
    }
}


// Shadow bus accesses to the $C800-$CFFF memory space
void videx_vterm_shadow_c8xx(bool is_write, uint_fast16_t address, uint_fast8_t value) {
    if(!videx_vterm_mem_selected)
        return;

    if(address < 0xcc00)
        return;  // ignore ROM reads

    if(address < 0xce00) {
        // this is the window into the card's video RAM
        if(is_write) {
            unsigned int vaddr = ((unsigned int)videx_banknum << 9) + (address & 0x01ff);
            videx_vram[vaddr] = value;
        }
    } else {
        // accesses to $CE00-$CFFF deactivates the card's memory
        videx_vterm_mem_selected = false;
    }
}


void videx_vterm_update_flasher() {
    uint64_t now = time_us_64();
    if(now > videx_next_flash_tick) {
        switch((videx_crtc_regs[10] >> 5) & 0x03) {
        case 0:
            // non-blinking cursor
            videx_cursor_mask = 0xff;
            videx_next_flash_tick = now + 312500u;
            break;
        case 1:
            // no cursor
            videx_cursor_mask = 0;
            videx_next_flash_tick = now + 312500u;
            break;
        case 2:
            // blink, 1/16th field rate
            videx_cursor_mask ^= 0xff;
            videx_next_flash_tick = now + 625000u;
            break;
        case 3:
            // blink, 1/32th field rate
            videx_cursor_mask ^= 0xff;
            videx_next_flash_tick = now + 312500u;
            break;
        }
    }
}


static inline uint_fast16_t char_videx_text_bits(uint_fast8_t ch, uint_fast8_t glyph_line, bool has_cursor) {
    uint_fast8_t bits;
    if(ch < 0x80) {
        bits = videx_normal[((uint_fast16_t)ch << 4) + glyph_line];
    } else {
        bits = videx_inverse[((uint_fast16_t)(ch & 0x7f) << 4) + glyph_line];
    }

    if(has_cursor && glyph_line >= CURSOR_LINE_START && glyph_line <= CURSOR_LINE_END) {
        bits ^= videx_cursor_mask;
    }

    return bits;
}


static void render_videx_text_line(unsigned int line) {
    const uint32_t bg_color = soft_monochrom ? mono_bg_color : ntsc_palette[0];
    const uint32_t fg_color = soft_monochrom ? mono_fg_color : ntsc_palette[15];
    uint32_t bits_to_pixelpair[4] = {
        (bg_color << 16) | bg_color,
        (bg_color << 16) | fg_color,
        (fg_color << 16) | bg_color,
        (fg_color << 16) | fg_color,
    };

    const uint vstart = ((videx_crtc_regs[12] & 0x3f) << 8) | videx_crtc_regs[13];
    const uint vcursor = ((videx_crtc_regs[14] & 0x3f) << 8) | videx_crtc_regs[15];

    for(uint glyph_line = 0; glyph_line < 9; glyph_line++) {
        struct vga_scanline *sl = vga_prepare_scanline();
        uint sl_pos = 0;

        // Note: Videx characters are 8 pixels wide so 80 columns fills an entire 640 pixel VGA line and
        // no left-padding is needed

        for(uint col = 0; col < 80; col++) {
            // Grab 8 pixels from the next character
            uint crtc_mem_offset = ((line * 80) + col) + vstart;
            bool has_cursor = (vcursor == crtc_mem_offset);
            uint_fast8_t ch = videx_vram[crtc_mem_offset % sizeof(videx_vram)];

            uint_fast16_t bits = char_videx_text_bits(ch, glyph_line, has_cursor);

            // Render each pair of bits into a pair of pixels, least significant bit first
            for(int i = 0; i < 4; i++) {
                sl->data[sl_pos] = bits_to_pixelpair[bits & 0x03];
                sl_pos++;
                bits >>= 2;
            }
        }

        if(soft_scanline_emulation) {
            // Just insert a blank scanline between each rendered scanline
            sl->data[sl_pos++] = THEN_WAIT_HSYNC;
        } else {
            sl->repeat_count = 1;
        }
        sl->length = sl_pos;
        vga_submit_scanline(sl);
    }
}


void render_videx_text() {
    vga_prepare_frame();
    // Skip 25 lines to center vertically
    vga_skip_lines(25);

    for(int line = 0; line < 24; line++) {
        render_videx_text_line(line);
    }
}
