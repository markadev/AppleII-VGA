#include <hardware/pio.h>
#include "config.h"
#include "vga.pio.h"
#include "vga.h"


// TODO verify these constants generate the right timing
#define HSYNC_TIMING_VALUE ((800 / 8) - 30)
#define VSYNC_TIMING_VALUE (525 - 3)


static void vga_hsync_setup(PIO pio) {
    uint program_offset = pio_add_program(pio, &vga_hsync_program);
    uint sm = pio_claim_unused_sm(pio, true);

    pio_sm_config c = vga_hsync_program_get_default_config(program_offset);
    sm_config_set_clkdiv(&c, CONFIG_SYSCLOCK / (25.175/8));  // approximate 25.175/8 MHz

    // Map the state machine's OUT pin group to the sync signal pin
    sm_config_set_out_pins(&c, CONFIG_PIN_HSYNC, 1);
    sm_config_set_set_pins(&c, CONFIG_PIN_HSYNC, 1);

    // Configure the pins as outputs & connect to the PIO
    pio_sm_set_consecutive_pindirs(pio, sm, CONFIG_PIN_HSYNC, 1, true);
    pio_gpio_init(pio, CONFIG_PIN_HSYNC);

    // Load our configuration, and jump to the start of the program
    pio_sm_init(pio, sm, program_offset, &c);
    pio_sm_set_enabled(pio, sm, true);

    // Push in the timing loop value
    pio_sm_put_blocking(pio, sm, HSYNC_TIMING_VALUE);
}

static void vga_vsync_setup(PIO pio) {
    uint program_offset = pio_add_program(pio, &vga_vsync_program);
    uint sm = pio_claim_unused_sm(pio, true);

    pio_sm_config c = vga_vsync_program_get_default_config(program_offset);
    sm_config_set_clkdiv(&c, CONFIG_SYSCLOCK / (25.175/8));  // approximate 25.175/8 MHz

    // Map the state machine's OUT pin group to the sync signal pin
    sm_config_set_out_pins(&c, CONFIG_PIN_VSYNC, 1);
    sm_config_set_set_pins(&c, CONFIG_PIN_VSYNC, 1);

    // Configure the pins as outputs & connect to the PIO
    pio_sm_set_consecutive_pindirs(pio, sm, CONFIG_PIN_VSYNC, 1, true);
    pio_gpio_init(pio, CONFIG_PIN_VSYNC);

    // Load our configuration, and jump to the start of the program
    pio_sm_init(pio, sm, program_offset, &c);
    pio_sm_set_enabled(pio, sm, true);

    // Push in the timing loop value
    pio_sm_put_blocking(pio, sm, VSYNC_TIMING_VALUE);
}

static uint vga_data_setup(PIO pio) {
    uint program_offset = pio_add_program(pio, &vga_data_program);
    uint sm = pio_claim_unused_sm(pio, true);

    pio_sm_config c = vga_data_program_get_default_config(program_offset);
    sm_config_set_clkdiv(&c, CONFIG_SYSCLOCK / (2*25.175));  // approximate 2*25.175MHz

    // Map the state machine's OUT pin group to the data pins
    sm_config_set_out_pins(&c, CONFIG_PIN_RGB_BASE, 9);
    sm_config_set_set_pins(&c, CONFIG_PIN_RGB_BASE, 9);

    // Enable autopull every 14 bits  (9 data + 5 jump bits)
    sm_config_set_out_shift(&c, true, true, 14);

    // Set join the state machine FIFOs to double the TX fifo size
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);

    // Configure the pins as outputs & connect to the PIO
    pio_sm_set_consecutive_pindirs(pio, sm, CONFIG_PIN_RGB_BASE, 9, true);
    for(int i=0; i < 9; i++) {
        pio_gpio_init(pio, CONFIG_PIN_RGB_BASE+i);
    }

    // Load our configuration, and jump to the start of the program
    pio_sm_init(pio, sm, program_offset, &c);
    pio_sm_set_enabled(pio, sm, true);

    return sm;
}

void vga_init() {
    vga_data_setup(pio0);
    vga_vsync_setup(pio0);
    vga_hsync_setup(pio0);
}
