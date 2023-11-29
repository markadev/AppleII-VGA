#include "render.h"

#include <pico/stdlib.h>
#include "buffers.h"
#include "colors.h"
#include "textfont.h"
#include "vga.h"


static uint_fast32_t text_flasher_mask = 0;
static uint64_t next_flash_tick = 0;


void update_text_flasher() {
    uint64_t now = time_us_64();
    if(now > next_flash_tick) {
        text_flasher_mask ^= 0xff;
        next_flash_tick = now + 250000u;
    }
}


static inline uint __time_critical_func(text_line_to_mem_offset)(uint line) {
    return ((line & 0x7) << 7) + (((line >> 3) & 0x3) * 40);
}


static inline uint_fast8_t __time_critical_func(char_text_bits)(uint_fast8_t ch, uint_fast8_t glyph_line) {
#ifdef APPLE_MODEL_IIE
    // Apple IIe character ROM addressing, see "Understanding the Apple IIe" table 8.3 on page 8-14
    //  characters 0x00-0x3f: direct mapping to ROM address
    //  characters 0x40-0x7f (ALTCHARSET=1): direct mapping to ROM address (for mousetext & inverse lower set)
    //  characters 0x40-0x7f (ALTCHARSET=0): map to 0x00-0x3f or 0x80-0xbf based on the flasher bit
    //  characters 0x80-0xff: direct mapping to ROM address
    if((ch >= 0x40) && (ch <= 0x7f) && !soft_altcharset) {
        ch = (ch & 0x3f) | (text_flasher_mask & 0x80);
    }
#endif

    uint_fast8_t bits = character_rom[((uint_fast16_t)ch << 3) + glyph_line];

#ifdef APPLE_MODEL_IIPLUS
    // For II+ characters:
    //  characters 0x00-0x7f: direct mapping to ROM address; when O7=1 then invert the output based on the flasher
    //  characters 0x80-0xff: direct mapping to ROM address
    if((ch < 0x80) && (bits & 0x80)) {
        // flashing character
        bits ^= text_flasher_mask;
    }
#endif

    return bits & 0x7f;
}


void __time_critical_func(render_text)() {
    vga_prepare_frame();
    // Skip 48 lines to center vertically
    vga_skip_lines(48);

    for(int line = 0; line < 24; line++) {
        render_text_line(line);
    }
}


void __time_critical_func(render_text_line)(unsigned int line) {
    const uint32_t bg_color = soft_monochrom ? mono_bg_color : ntsc_palette[0];
    const uint32_t fg_color = soft_monochrom ? mono_fg_color : ntsc_palette[15];
    uint32_t bits_to_pixelpair[4] = {
        (bg_color << 16) | bg_color,
        (bg_color << 16) | fg_color,
        (fg_color << 16) | bg_color,
        (fg_color << 16) | fg_color,
    };

    const uint line_offset = text_line_to_mem_offset(line);

    const uint8_t *page_main = is_page2_display_enabled() ? text_mainmem_page2 : text_mainmem_page1;
    const uint8_t *line_main = page_main + line_offset;

    const uint8_t *line_aux = 0;
    if(soft_80col) {
        // Read every other character from the aux memory bank in 80 column mode
        const uint8_t *page_aux = is_page2_display_enabled() ? text_auxmem_page2 : text_auxmem_page1;
        line_aux = page_aux + line_offset;
    } else {
        // else just double the pixel width in 40 column mode
        for(int i = 0; i < 4; i++) {
            bits_to_pixelpair[i] |= (THEN_EXTEND_1 | (THEN_EXTEND_1 << 16));
        }
    }

    for(uint glyph_line = 0; glyph_line < 8; glyph_line++) {
        struct vga_scanline *sl = vga_prepare_scanline();
        uint sl_pos = 0;

        // Pad 40 pixels on the left to center horizontally
        sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels
        sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels
        sl->data[sl_pos++] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels

        for(uint col = 0; col < 40;) {
            // Grab 14 pixels from the next two characters. If an aux memory bank was provided (80 column mode is on)
            // then the first character comes from that, otherwise both characters just come from main memory.
            uint_fast8_t char_a = (line_aux != NULL) ? line_aux[col] : line_main[col++];
            uint_fast8_t char_b = line_main[col++];

            uint_fast16_t bits =
                ((uint_fast16_t)char_text_bits(char_b, glyph_line) << 7) | (uint_fast16_t)char_text_bits(char_a, glyph_line);

            // Render each pair of bits into a pair of pixels, least significant bit first
            for(int i = 0; i < 7; i++) {
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
