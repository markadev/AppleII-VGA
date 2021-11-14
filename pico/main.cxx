#include <pico/stdlib.h>
#include "vga.h"
#include "config.h"


static uint16_t scanline_buffer[648];


int main() {
    // Adjust system clock for better dividing into other clocks
    set_sys_clock_khz(CONFIG_SYSCLOCK*1000, true);

    vga_init();

    uint scanline_size = 1;

    // red
    for(int shade=7; shade >= 0; shade--) {
        for(int i=0; i < 8; i++) {
            scanline_buffer[scanline_size++] = shade << 6;
        }
    }

    // green
    for(int shade=7; shade >= 0; shade--) {
        for(int i=0; i < 8; i++) {
            scanline_buffer[scanline_size++] = shade << 3;
        }
    }

    // blue
    for(int shade=7; shade >= 0; shade--) {
        for(int i=0; i < 8; i++) {
            scanline_buffer[scanline_size++] = shade;
        }
    }

    // white vertical lines
    for(int i=192; i < 640; i++) {
        scanline_buffer[scanline_size++] = (i & 1) ? 0x1ff : 0;
    }

    // beam off
    scanline_buffer[scanline_size++] = 0;

    while(1) {
        for(uint i=0; i < 200; i++) {
            if(i == 0) {
                scanline_buffer[0] = 0|THEN_WAIT_VSYNC;
            } else {
                scanline_buffer[0] = 0|THEN_WAIT_HSYNC;
            }
            if(i < 35) {
                // skip 35 lines of vsync + vertical back porch
                vga_send_scanline(scanline_buffer, 1);
            } else {
                vga_send_scanline(scanline_buffer, scanline_size);
            }
        }
    }
}
