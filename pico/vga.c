#include <hardware/dma.h>
#include <hardware/pio.h>
#include "config.h"
#include "vga.pio.h"
#include "vga.h"


#define PIXEL_FREQ 25.2/*MHz*/
#define PIXELS_PER_LINE 800
#define LINES_PER_FRAME 525

#define HSYNC_TIMING_VALUE (((PIXELS_PER_LINE) / 8) - 30)
#define VSYNC_TIMING_VALUE ((LINES_PER_FRAME) - 4)


enum {
    VGA_HSYNC_SM = 0,
    VGA_VSYNC_SM = 1,
    VGA_DATA_SM = 2,
};


static uint vga_dma_channel;


static void vga_hsync_setup(PIO pio, uint sm) {
    uint program_offset = pio_add_program(pio, &vga_hsync_program);
    pio_sm_claim(pio, sm);

    pio_sm_config c = vga_hsync_program_get_default_config(program_offset);
    sm_config_set_clkdiv(&c, CONFIG_SYSCLOCK * 8 / PIXEL_FREQ);  // 1/8 * PIXEL_FREQ

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
    sm_config_set_clkdiv(&c, CONFIG_SYSCLOCK * 8 / PIXEL_FREQ);  // 1/8 * PIXEL_FREQ

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
    sm_config_set_clkdiv(&c, CONFIG_SYSCLOCK / (2*PIXEL_FREQ));  // 2 * PIXEL_FREQ

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
    // Setup the PIO state machines
    vga_hsync_setup(CONFIG_VGA_PIO, VGA_HSYNC_SM);
    vga_vsync_setup(CONFIG_VGA_PIO, VGA_VSYNC_SM);
    vga_data_setup(CONFIG_VGA_PIO, VGA_DATA_SM);

    // Setup the DMA channel for writing to the data PIO state machine
    vga_dma_channel = dma_claim_unused_channel(true);
    dma_channel_config c = dma_channel_get_default_config(vga_dma_channel);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_16);
    channel_config_set_dreq(&c, pio_get_dreq(CONFIG_VGA_PIO, VGA_DATA_SM, true));
    dma_channel_configure(vga_dma_channel, &c, &CONFIG_VGA_PIO->txf[VGA_DATA_SM], NULL, 0, false);

    // Enable all state machines in sync to ensure their instruction cycles line up
    pio_enable_sm_mask_in_sync(CONFIG_VGA_PIO, (1 << VGA_HSYNC_SM) | (1 << VGA_VSYNC_SM) | (1 << VGA_DATA_SM));
}

void vga_send_scanline(uint16_t *buf, uint len) {
    dma_channel_transfer_from_buffer_now(vga_dma_channel, buf, len);
    dma_channel_wait_for_finish_blocking(vga_dma_channel);
}
