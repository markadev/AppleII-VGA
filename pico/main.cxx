#include <pico/stdlib.h>
#include <hardware/dma.h>
#include <hardware/pio.h>
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


    int dma_chan = dma_claim_unused_channel(true);
    dma_channel_config c = dma_channel_get_default_config(dma_chan);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_16);
    channel_config_set_read_increment(&c, true);
    channel_config_set_write_increment(&c, false);
    channel_config_set_dreq(&c, pio_get_dreq(CONFIG_VGA_PIO, 2/*FIXME*/, true));
    dma_channel_configure(dma_chan, &c,
        &CONFIG_VGA_PIO->txf[2/*FIXME*/],              // Destination pointer
        NULL,       // Source pointer
        0,          // Number of transfers
        false       // Don't start yet
    );

    while(1) {
        for(uint i=0; i < 200; i++) {
            if(i == 0) {
                scanline_buffer[0] = 0|THEN_WAIT_VSYNC;
            } else {
                scanline_buffer[0] = 0|THEN_WAIT_HSYNC;
            }
            if(i < 35) {
                // skip 35 lines of vsync + vertical back porch
                dma_channel_transfer_from_buffer_now(dma_chan, scanline_buffer, 1);
            } else {
                dma_channel_transfer_from_buffer_now(dma_chan, scanline_buffer, scanline_size);
            }

            // Wait for the DMA transfer to complete
            dma_channel_wait_for_finish_blocking(dma_chan);
        }
    }
}
