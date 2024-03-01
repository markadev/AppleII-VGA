#include "videx_vterm.h"

#include <pico/stdlib.h>
#include "buffers.h"
#include "colors.h"
#include "textfont/textfont.h"
#include "vga.h"


volatile bool videx_vterm_80col_enabled;
volatile bool videx_vterm_mem_selected;

static uint8_t videx_vram[2048];
static uint8_t videx_bankSLOT;     // selected memory bank
static uint8_t videx_regvalSLOT;   // selected CRTC register
static uint8_t videx_regSLOT[17];  // registers of the CRT-controller

static uint8_t videx_upperSLOT, videx_lowerSLOT;  // cursor size
static uint8_t videx_blink_mode, videx_blink_cursor;

static uint_fast32_t videx_text_flasher_mask = 0;
static uint64_t videx_next_flash_tick = 0;


static void card_videx_modifySLOT();


void videx_vterm_init() {
    // initializing registers
    videx_bankSLOT = 0;
    videx_regvalSLOT = 0;
    videx_regSLOT[0] = 0x7b;
    videx_regSLOT[1] = 0x50;
    videx_regSLOT[2] = 0x62;
    videx_regSLOT[3] = 0x29;
    videx_regSLOT[4] = 0x1b;
    videx_regSLOT[5] = 0x08;
    videx_regSLOT[6] = 0x18;
    videx_regSLOT[7] = 0x19;
    videx_regSLOT[8] = 0x00;
    videx_regSLOT[9] = 0x08;
    videx_regSLOT[10] = 0xc0;
    videx_regSLOT[11] = 0x08;
    videx_regSLOT[12] = 0x00;
    videx_regSLOT[13] = 0x00;
    videx_regSLOT[14] = 0x00;
    videx_regSLOT[15] = 0x00;
    videx_regSLOT[16] = 0x00;
    videx_upperSLOT = 0;
    videx_lowerSLOT = 8;

    card_videx_modifySLOT();
}


// Shadow accesses to card registers in $C0n0 - $C0nF range
void videx_vterm_shadow_register(bool is_write, uint_fast16_t address, uint_fast8_t data) {
    // define current memory bank
    videx_bankSLOT = (address & 0x000c) >> 2;

    if(!is_write)
        return;

    if(address & 0x0001) {
        // set current register value
        videx_regSLOT[videx_regvalSLOT] = data;

        // 10: Cursor upper, 11: Cursor lower
        if((videx_regvalSLOT == 10) || (videx_regvalSLOT == 11)) {
            card_videx_modifySLOT();
        }
    } else {
        // define current register
        videx_regvalSLOT = data;
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
            unsigned int vaddr = ((unsigned int)videx_bankSLOT << 9) + (address & 0x01ff);
            videx_vram[vaddr] = value;
        }
    } else {
        // accesses to $CE00-$CFFF deactivates the card's memory
        videx_vterm_mem_selected = false;
    }
}


static void card_videx_modifySLOT() {
    videx_blink_cursor = (videx_regSLOT[10] & 0x40) > 0;
    videx_blink_mode = (videx_regSLOT[10] & 0x20) > 0;

    // If videx_blink_cursor is false then
    // videx_blink_mode determines if there is a cursor displayed (false) or not (true).

    // If videx_blink_cursor is true then
    // videx_blink_mode determines blink rate
    //  - false = 1/16th field rate blink
    //  - true = 1/32th field rate blink

    // glyph start
    videx_upperSLOT = videx_regSLOT[10] & 0xf;
    if(videx_upperSLOT > 11)
        videx_upperSLOT = 11;

    // glyph end
    videx_lowerSLOT = videx_regSLOT[11] & 0xf;
    if(videx_lowerSLOT > 11)
        videx_lowerSLOT = 11;

    // if the setting is ridiculous
    if(videx_upperSLOT >= videx_lowerSLOT) {
        videx_upperSLOT = 0;
        videx_lowerSLOT = 11;
    }
}


void update_videx_text_flasher() {
    uint64_t now = time_us_64();
    if(now > videx_next_flash_tick) {
        videx_text_flasher_mask ^= 0xff;

        // If videx_blink_cursor is true then
        // videx_blink_mode determines blink rate
        //  - false = 1/16th field rate blink 62500u
        //  - true = 1/32th field rate blink 31250u

        if(videx_blink_mode)
            videx_next_flash_tick = now + 312500u;
        else
            videx_next_flash_tick = now + 625000u;
    }
}


static inline uint_fast16_t char_videx_text_bits(uint_fast8_t ch, uint_fast8_t glyph_line, bool has_cursor) {
    uint_fast8_t bits;
    if(ch < 0x80) {
        bits = videx_normal[((uint_fast16_t)ch << 4) + glyph_line];
    } else {
        bits = videx_inverse[((uint_fast16_t)(ch & 0x7f) << 4) + glyph_line];
    }

    // If videx_blink_cursor is false then
    // videx_blink_mode determines if there is a cursor displayed (false) or not (true).

    // If videx_blink_cursor is true then
    // videx_blink_mode determines blink rate
    //  - false = 1/16th field rate blink
    //  - true = 1/32th field rate blink
    if(has_cursor && videx_upperSLOT <= glyph_line && videx_lowerSLOT >= glyph_line) {
        // flashing character
        if(videx_blink_cursor) {
            bits ^= videx_text_flasher_mask;
        } else if(!videx_blink_mode) {
            bits ^= 0xff;
        }
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

    const uint vstart = ((videx_regSLOT[12] << 8) | videx_regSLOT[13]);
    const uint vcursor = ((videx_regSLOT[14] << 8) | videx_regSLOT[15]);

    for(uint glyph_line = 0; glyph_line < 9; glyph_line++) {
        struct vga_scanline *sl = vga_prepare_scanline();
        uint sl_pos = 0;

        // Note: Videx characters are 8 pixels wide so 80 columns fills an entire 640 pixel VGA line and
        // no left-padding is needed

        for(uint col = 0; col < 80; col++) {
            // Grab 8 pixels from the next character
            uint line_offset = ((line * 80) + col) + vstart;
            uint vram_offset = line_offset % sizeof(videx_vram);
            bool has_cursor = (vcursor == line_offset);
            uint_fast8_t ch = videx_vram[vram_offset];

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
