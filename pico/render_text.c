#include <pico/stdlib.h>
#include "render.h"
#include "textfont.h"
#include "vga.h"


static uint_fast32_t text_flasher_mask = 0;
static absolute_time_t next_flash_tick = 0;


void update_text_flasher() {
    absolute_time_t now = time_us_64();
    if(now > next_flash_tick) {
        text_flasher_mask ^= 0xff;
        next_flash_tick = now + 250000u;
    }
}


static inline uint_fast8_t __time_critical_func(char_text_bits)(uint_fast8_t ch, uint_fast8_t glyph_line) {
    uint_fast8_t bits = default_font[((uint_fast16_t)ch << 3) | glyph_line];
    if(ch & 0x80) {
        // normal character
        return bits & 0x7f;
    }

    if((bits & 0x80) == 0) {
        // inverse character
        return bits ^ 0x7f;
    } else {
        // flashing character
        return (bits ^ text_flasher_mask) & 0x7f;
    }
}


void __time_critical_func(render_text)(uint8_t *page) {
    vga_prepare_frame();

    // Skip 48 lines to center vertically
    struct vga_scanline *skip_sl = vga_prepare_scanline();
    for(int i=0; i < 48; i++) {
        skip_sl->data[i] = (uint32_t)THEN_WAIT_HSYNC << 16;
    }
    skip_sl->length = 48;
    vga_submit_scanline(skip_sl);

    for(int line=0; line < 24; line++) {
        render_text_line(page, line);
    }
}


void __time_critical_func(render_text_line)(uint8_t *page, unsigned int line) {
    const uint8_t *line_buf = &page[((line & 0x7) << 7) + (((line >> 3) & 0x3) * 40)];

    for(uint glyph_line=0; glyph_line < 8; glyph_line++) {
        struct vga_scanline *sl = vga_prepare_scanline();
        uint sl_pos = 0;

        // Pad 40 pixels on the left to center horizontally
        while(sl_pos < 40/8) {
            sl->data[sl_pos] = (0|THEN_EXTEND_3) | ((0|THEN_EXTEND_3) << 16); // 8 pixels per word
            sl_pos++;
        }

        for(uint col=0; col < 40; ) {
            // Grab 14 pixels from the next two characters
            uint32_t bits_a = char_text_bits(line_buf[col], glyph_line);
            col++;
            uint32_t bits_b = char_text_bits(line_buf[col], glyph_line);
            col++;

            uint32_t bits = (bits_a << 7) | bits_b;

            // Translate each pair of bits into a pair of pixels
            for(int i=0; i < 7; i++) {
                uint32_t pixeldata = (bits & 0x2000) ? (0x1ff|THEN_EXTEND_1) : (0|THEN_EXTEND_1);
                pixeldata |= (bits & 0x1000) ?
                    ((uint32_t)0x1ff|THEN_EXTEND_1) << 16 :
                    ((uint32_t)0|THEN_EXTEND_1) << 16;
                bits <<= 2;

                sl->data[sl_pos] = pixeldata;
                sl_pos++;
            }
        }

        sl->length = sl_pos;
        sl->repeat_count = 1;
        vga_submit_scanline(sl);
    }
}
