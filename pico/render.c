#include <pico/stdlib.h>
#include <hardware/timer.h>
#include "buffers.h"
#include "textfont.h"
#include "vga.h"


static uint_fast32_t text_flasher_mask = 0;


static void __not_in_flash_func(render_testpattern)() {
    vga_prepare_frame();

    for(uint line=0; line < VGA_HEIGHT; line++) {
        struct vga_scanline *sl = vga_prepare_scanline();

        if(line == 0 || line == VGA_HEIGHT-1) {
            for(uint i=0; i < VGA_WIDTH/4; i++) {
                sl->data[i] = ((uint32_t)0x1ff | THEN_EXTEND_1) | (((uint32_t)0x1ff | THEN_EXTEND_1) << 16);
            }
            sl->length = VGA_WIDTH/4; // number of 32-bit words
        } else if(line < 120) {
            for(uint32_t i=0; i < 256; i+=2) {
                uint32_t w = ((i & 0x1ff) | THEN_EXTEND_1) |
                    (((i+1 & 0x1ff) | THEN_EXTEND_1) << 16);
                sl->data[i/2] = w;
            }
            sl->length = 256/2;
        } else if(line < 140) {
            for(uint i=0; i < VGA_WIDTH/4; i++) {
                sl->data[i] = (uint32_t)0x1ff;
            }
            for(uint i=VGA_WIDTH/4; i < VGA_WIDTH/2; i++) {
                sl->data[i] = (uint32_t)0x1ff << 16;
            }
            sl->length = VGA_WIDTH/2;
            sl->repeat_count = 19;
            line += 19;
        }

        vga_submit_scanline(sl);
    }
}


static inline uint_fast8_t char_text_bits(uint_fast8_t ch, uint_fast8_t glyph_line) {
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

static void render_text(uint8_t *page) {
    vga_prepare_frame();

    for(int line=0; line < 24; line++) {
        const uint8_t *line_buf = &page[((line & 0x7) << 7) + (((line >> 3) & 0x3) * 40)];

        for(int glyph_line=0; glyph_line < 8; glyph_line++) {
            struct vga_scanline *sl = vga_prepare_scanline();
            uint sl_pos = 0;

            for(int col=0; col < 40; ) {
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
}


static void tick_text_flasher() {
    static absolute_time_t next_flash_tick = 0;

    absolute_time_t now = time_us_64();
    if(now > next_flash_tick) {
        text_flasher_mask ^= 0xff;
        next_flash_tick = now + 250000u;
    }
}

void render_loop() {
    while(1) {
        tick_text_flasher();

        //render_testpattern();
        render_text(text_page);
    }
}
