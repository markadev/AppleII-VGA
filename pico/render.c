#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include "vga.h"


// XXX
#define DEBUG_PIN 0


static uint16_t scanline_buffers[2][648];

// TODO make proper defines in vga.h
#define NUM_PRELINES 35 /*XXX*/


void __not_in_flash_func(render_loop)() {
    gpio_init(DEBUG_PIN);
    gpio_set_dir(DEBUG_PIN, GPIO_OUT);

    while(1) {
        for(uint scanline=0; scanline < 100; scanline++) {
            uint16_t *scanline_buffer = scanline_buffers[scanline & 0x1];
            uint scanline_size = 0;

            if(scanline < NUM_PRELINES) {
                // blank area
                if(scanline == 0) {
                    scanline_buffer[0] = 0;
                    scanline_buffer[1] = 0|THEN_WAIT_VSYNC;
                } else {
                    scanline_buffer[0] = 0;
                    scanline_buffer[1] = 0|THEN_WAIT_HSYNC;
                }
                scanline_size = 2;
            } else if(scanline < NUM_PRELINES+480) {
                gpio_put(DEBUG_PIN, 1);
                scanline_buffer[0] = 0|THEN_WAIT_HSYNC;
                for(uint32_t i=2; i < 256; i+=2) {
                    //scanline_buffer[i] = (i & 0x1ff) | THEN_EXTEND_1;
                    // TODO 32-bit stores are measurably 2X faster than 16-bit stores
                    uint32_t w = (((i+1 & 0x1ff) | THEN_EXTEND_1) << 16) | ((i & 0x1ff) | THEN_EXTEND_1);
                    *((uint32_t *)(&scanline_buffer[i])) = w;
                }

                scanline_buffer[256] = 0;  // beam off
                scanline_buffer[257] = 0;  // beam off
                scanline_size = 258;
                gpio_put(DEBUG_PIN, 0);
            }

            vga_send_scanline(scanline_buffer, scanline_size);
        }
    }
}
