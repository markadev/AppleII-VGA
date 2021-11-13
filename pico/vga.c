#include <hardware/pio.h>
#include "config.h"
#include "vga.pio.h"
#include "vga.h"

#define DEBUG_TIMING 0

#define PIXELS_PER_LINE 800
#define LINES_PER_FRAME 525

#define HSYNC_TIMING_VALUE (((PIXELS_PER_LINE) / 8) - 30)
#define VSYNC_TIMING_VALUE ((LINES_PER_FRAME) - 4)

static const uint VGA_HSYNC_SM = 0;
static const uint VGA_VSYNC_SM = 1;
static const uint VGA_DATA_SM = 2;


static void vga_hsync_setup(PIO pio, uint sm) {
    uint program_offset = pio_add_program(pio, &vga_hsync_program);
    pio_sm_claim(pio, sm);

    pio_sm_config c = vga_hsync_program_get_default_config(program_offset);
#if DEBUG_TIMING
    sm_config_set_clkdiv(&c, CONFIG_SYSCLOCK*8);  // 1 microsecond per pixel
#else
    sm_config_set_clkdiv(&c, CONFIG_SYSCLOCK / (25.175/8));  // approximate 25.175/8 MHz
#endif

    // Map the state machine's OUT pin group to the sync signal pin
    sm_config_set_out_pins(&c, CONFIG_PIN_HSYNC, 1);
    sm_config_set_set_pins(&c, CONFIG_PIN_HSYNC, 1);

    // Configure the pins as outputs & connect to the PIO
    pio_sm_set_consecutive_pindirs(pio, sm, CONFIG_PIN_HSYNC, 1, true);
    pio_gpio_init(pio, CONFIG_PIN_HSYNC);

    // Load the configuration and push in the timing loop value
    pio_sm_init(pio, sm, program_offset, &c);
    pio_sm_put_blocking(pio, sm, HSYNC_TIMING_VALUE);
}

static void vga_vsync_setup(PIO pio, uint sm) {
    uint program_offset = pio_add_program(pio, &vga_vsync_program);
    pio_sm_claim(pio, sm);

    pio_sm_config c = vga_vsync_program_get_default_config(program_offset);
#if DEBUG_TIMING
    sm_config_set_clkdiv(&c, CONFIG_SYSCLOCK*8);  // 1 microsecond per pixel
#else
    sm_config_set_clkdiv(&c, CONFIG_SYSCLOCK / (25.175/8));  // approximate 25.175/8 MHz
#endif

    // Map the state machine's OUT pin group to the sync signal pin
    sm_config_set_out_pins(&c, CONFIG_PIN_VSYNC, 1);
    sm_config_set_set_pins(&c, CONFIG_PIN_VSYNC, 1);

    // Configure the pins as outputs & connect to the PIO
    pio_sm_set_consecutive_pindirs(pio, sm, CONFIG_PIN_VSYNC, 1, true);
    pio_gpio_init(pio, CONFIG_PIN_VSYNC);

    // Load the configuration and push in the timing loop value
    pio_sm_init(pio, sm, program_offset, &c);
    pio_sm_put_blocking(pio, sm, VSYNC_TIMING_VALUE);
}

static void vga_data_setup(PIO pio, uint sm) {
    uint program_offset = pio_add_program(pio, &vga_data_program);
    pio_sm_claim(pio, sm);

    pio_sm_config c = vga_data_program_get_default_config(program_offset);
#if DEBUG_TIMING
    sm_config_set_clkdiv(&c, CONFIG_SYSCLOCK/2);  // 1 microsecond per pixel
#else
    sm_config_set_clkdiv(&c, CONFIG_SYSCLOCK / (2*25.175));  // approximate 2*25.175MHz
#endif

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

    // Load the configuration, starting execution at 'wait_vsync'
    pio_sm_init(pio, sm, program_offset+vga_data_offset_wait_vsync, &c);
}

void vga_init() {
    vga_hsync_setup(CONFIG_VGA_PIO, VGA_HSYNC_SM);
    vga_vsync_setup(CONFIG_VGA_PIO, VGA_VSYNC_SM);
    vga_data_setup(CONFIG_VGA_PIO, VGA_DATA_SM);

    // Enable all state machines in sync to ensure their instruction cycles line up
    pio_enable_sm_mask_in_sync(CONFIG_VGA_PIO, (1 << VGA_HSYNC_SM) | (1 << VGA_VSYNC_SM) | (1 << VGA_DATA_SM));

#if DEBUG_TIMING
    // output data byte patterns
    while(1) {
        const uint THEN_WAIT_VSYNC = (vga_data_offset_wait_vsync << 9);
        const uint THEN_WAIT_HSYNC = (vga_data_offset_wait_hsync << 9);

#if 1
        pio_sm_put_blocking(CONFIG_VGA_PIO, VGA_DATA_SM, 0|THEN_WAIT_VSYNC);
#else
        pio_sm_put_blocking(CONFIG_VGA_PIO, VGA_DATA_SM, 0|THEN_WAIT_HSYNC);
#endif
        for(int i=0; i < 8; i++) {
            pio_sm_put_blocking(CONFIG_VGA_PIO, VGA_DATA_SM, 0x1ff);
            pio_sm_put_blocking(CONFIG_VGA_PIO, VGA_DATA_SM, 0x000);
        }
    }
#endif
}

void vga_data_out(uint16_t data) {
    pio_sm_put_blocking(CONFIG_VGA_PIO, VGA_DATA_SM, data);
}
