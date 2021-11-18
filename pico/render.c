#include <string.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include "textfont.h"
#include "vga.h"


// XXX
#define DEBUG_PIN 0


// TODO make proper defines in vga.h
// TODO why is this 34 and not 35?
#define NUM_PRELINES 34 /*XXX*/


static uint8_t text_buffer[24][40];


static void __not_in_flash_func(render_testpattern)() {
    // TODO: create one 'scanline' with the vsync and all vertical back porch hsyncs
    // and encapsulate all this frame setup
    struct vga_scanline *sl = vga_prepare_scanline(true);
    memset(sl->data, 0, 128);
    sl->length = 128/4;
    vga_submit_scanline(sl);

    sl = vga_prepare_scanline(false);
    memset(sl->data, 0, 128);
    sl->length = 128/4;
    sl->repeat_count = NUM_PRELINES-2;
    vga_submit_scanline(sl);

    for(uint line=0; line < VGA_HEIGHT; line++) {
        struct vga_scanline *sl = vga_prepare_scanline(false);

        gpio_put(DEBUG_PIN, 1);

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
        } else {
            // TODO: make the hsync IRQ shorter so smaller placeholder scanlines can be used
            // to skip lines.
            memset(sl->data, 0, 128);
            sl->length = 128/4;
        }

        gpio_put(DEBUG_PIN, 0);

        vga_submit_scanline(sl);
    }
}


static void render_text() {
    struct vga_scanline *sl = vga_prepare_scanline(true);
    memset(sl->data, 0, 128);
    sl->length = 128/4;
    vga_submit_scanline(sl);

    sl = vga_prepare_scanline(false);
    memset(sl->data, 0, 128);
    sl->length = 128/4;
    sl->repeat_count = NUM_PRELINES-2;
    vga_submit_scanline(sl);

    for(int line=0; line < 24*8; line++) {
        uint8_t *line_buf = text_buffer[line >> 3];
        uint glyph_line = line & 0x7;

        sl = vga_prepare_scanline(false);
        uint sl_pos = 0;

        gpio_put(DEBUG_PIN, 1);

        for(int col=0; col < 40; col+=2) {
            uint char_a = line_buf[col] & 0x3f;
            uint char_b = line_buf[col+1] & 0x3f;
            uint32_t bits = ((uint32_t)default_font[char_a][glyph_line] << 7) | default_font[char_b][glyph_line];

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

        gpio_put(DEBUG_PIN, 0);

        sl->length = sl_pos;
        sl->repeat_count = 1;
        vga_submit_scanline(sl);
    }
}

void render_loop() {
    gpio_init(DEBUG_PIN);
    gpio_set_dir(DEBUG_PIN, GPIO_OUT);

    memset(text_buffer, 0x20, sizeof(text_buffer));
    strcpy(text_buffer[0], "APPLE ][+");
    strcpy(text_buffer[23], "LAST LINE");

    while(1) {
        //render_testpattern();
        render_text();
    }
}
