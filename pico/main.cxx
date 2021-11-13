#include <pico/stdlib.h>
#include "vga.h"
#include "config.h"


int main() {
    // Adjust system clock for better dividing into other clocks
    set_sys_clock_khz(CONFIG_SYSCLOCK*1000, true);

    vga_init();

    while(1) {
        for(uint i=0; i < 200; i++) {
            if(i == 0) {
                vga_data_out(0|THEN_WAIT_VSYNC);
            } else {
                vga_data_out(0|THEN_WAIT_HSYNC);
            }
            if(i < 35) {
                continue; // skip 35 lines of vsync + vertical back porch
            }

            // red
            for(int shade=7; shade >= 0; shade--) {
                vga_data_out(shade << 6);
                vga_data_out(shade << 6);
                vga_data_out(shade << 6);
                vga_data_out(shade << 6);
                vga_data_out(shade << 6);
                vga_data_out(shade << 6);
                vga_data_out(shade << 6);
                vga_data_out(shade << 6);
            }

            // green
            for(int shade=7; shade >= 0; shade--) {
                vga_data_out(shade << 3);
                vga_data_out(shade << 3);
                vga_data_out(shade << 3);
                vga_data_out(shade << 3);
                vga_data_out(shade << 3);
                vga_data_out(shade << 3);
                vga_data_out(shade << 3);
                vga_data_out(shade << 3);
            }

            // blue
            for(int shade=7; shade >= 0; shade--) {
                vga_data_out(shade);
                vga_data_out(shade);
                vga_data_out(shade);
                vga_data_out(shade);
                vga_data_out(shade);
                vga_data_out(shade);
                vga_data_out(shade);
                vga_data_out(shade);
            }
        }
    }
}
