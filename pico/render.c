#include <string.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include "vga.h"


// XXX
#define DEBUG_PIN 0


// TODO make proper defines in vga.h
// TODO why is this 34 and not 35?
#define NUM_PRELINES 34 /*XXX*/


void __not_in_flash_func(render_loop)() {
    gpio_init(DEBUG_PIN);
    gpio_set_dir(DEBUG_PIN, GPIO_OUT);

    while(1) {
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
}
