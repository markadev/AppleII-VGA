#include <pico/stdlib.h>
#include "vga.h"
#include "config.h"


int main() {
    // Adjust system clock for better dividing into other clocks
    set_sys_clock_khz(CONFIG_SYSCLOCK*1000, true);

    vga_init();

    while(1) {
    }
}
