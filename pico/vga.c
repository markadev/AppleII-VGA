#include <hardware/dma.h>
#include <hardware/irq.h>
#include <hardware/pio.h>
#include <hardware/sync.h>
#include "config.h"
#include "vga.pio.h"
#include "vga.h"


#define PIXEL_FREQ 25.2/*MHz*/
#define PIXELS_PER_LINE 800
#define LINES_PER_FRAME 525
#define LINES_IN_BACK_PORCH 33

#define HSYNC_TIMING_VALUE (((PIXELS_PER_LINE) / 8) - 23)
#define VSYNC_TIMING_VALUE ((LINES_PER_FRAME) - 4)

#define NUM_SCANLINE_BUFFERS 8

enum {
    VGA_HSYNC_SM = 0,
    VGA_VSYNC_SM = 1,
    VGA_DATA_SM = 2,
};

// The scanline flags form a simple state machine:
//    Initial state (0)
//         \/ prepare()
//        BUSY
//         \/ ready()
//      BUSY|READY
//         \/ first DMA started
//  BUSY|READY|STARTED
//         \/ last DMA completed
//    READY|STARTED
enum {
    FLAG_BUSY = 0x01,
    FLAG_READY = 0x02,
    FLAG_STARTED = 0x04,
};


static uint vga_dma_channel;

// Scanline queue. Scanlines are filled in from the head and are
// sent to the DMA engine from the tail.
static uint scanline_queue_head;
static uint scanline_queue_tail;
static struct vga_scanline scanline_queue[NUM_SCANLINE_BUFFERS];


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

    // Enable autopull every 32 bits  (2 x (9 data + 5 jump + 2 pad) bits)
    sm_config_set_out_shift(&c, true, true, 32);

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

// Start the DMA operation of the next scanline if it's ready.
//
// Must be called with the VGA spinlock held
static void trigger_ready_scanline_dma() {
    struct vga_scanline *active_scanline = &scanline_queue[scanline_queue_tail];

    if((active_scanline->_flags & (FLAG_BUSY|FLAG_READY|FLAG_STARTED)) == (FLAG_BUSY|FLAG_READY)) {
        active_scanline->_flags |= FLAG_STARTED;
        dma_channel_transfer_from_buffer_now(vga_dma_channel, &(active_scanline->_sync), active_scanline->length + 2);
    }
}

static void vga_dma_irq_handler() {
    spin_lock_t *lock = spin_lock_instance(CONFIG_VGA_SPINLOCK_ID);
    struct vga_scanline *active_scanline = &scanline_queue[scanline_queue_tail];

    // Ack the IRQ
    dma_hw->ints0 = 1u << vga_dma_channel;

    // Repeat the scanline as specified
    if(active_scanline->repeat_count) {
        active_scanline->repeat_count--;
        dma_channel_transfer_from_buffer_now(vga_dma_channel, &(active_scanline->_sync), active_scanline->length + 2);
        return;
    }

    // Mark the scanline done
    active_scanline->_flags &= ~(uint_fast8_t)FLAG_BUSY;

    const uint32_t irq_status = spin_lock_blocking(lock);
    scanline_queue_tail = (scanline_queue_tail + 1) % NUM_SCANLINE_BUFFERS;
    trigger_ready_scanline_dma();
    spin_unlock(lock, irq_status);
}


void vga_init() {
    spin_lock_claim(CONFIG_VGA_SPINLOCK_ID);
    spin_lock_init(CONFIG_VGA_SPINLOCK_ID);

    // Setup the PIO state machines
    vga_hsync_setup(CONFIG_VGA_PIO, VGA_HSYNC_SM);
    vga_vsync_setup(CONFIG_VGA_PIO, VGA_VSYNC_SM);
    vga_data_setup(CONFIG_VGA_PIO, VGA_DATA_SM);

    // Setup the DMA channel for writing to the data PIO state machine
    vga_dma_channel = dma_claim_unused_channel(true);
    dma_channel_config c = dma_channel_get_default_config(vga_dma_channel);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_32);
    channel_config_set_dreq(&c, pio_get_dreq(CONFIG_VGA_PIO, VGA_DATA_SM, true));
    dma_channel_configure(vga_dma_channel, &c, &CONFIG_VGA_PIO->txf[VGA_DATA_SM], NULL, 0, false);

    dma_channel_set_irq0_enabled(vga_dma_channel, true);
    irq_set_exclusive_handler(DMA_IRQ_0, vga_dma_irq_handler);
    irq_set_enabled(DMA_IRQ_0, true);

    // Enable all state machines in sync to ensure their instruction cycles line up
    pio_enable_sm_mask_in_sync(CONFIG_VGA_PIO, (1 << VGA_HSYNC_SM) | (1 << VGA_VSYNC_SM) | (1 << VGA_DATA_SM));
}

// Set up for a new display frame
void vga_prepare_frame() {
    // Populate a 'scanline' with multiple sync instructions to synchronize with the
    // vsync and then skip over the vertical back porch.
    struct vga_scanline *sl = vga_prepare_scanline();

    sl->_sync = (uint32_t)THEN_WAIT_VSYNC << 16;
    // FIXME: the number of hsyncs we have to wait for seems to be one too few
    // because the vsync is supposed to last two lines (we wait one) and THEN
    // the back porch lines need to be skipped.
    for(int i=0; i < LINES_IN_BACK_PORCH; i++) {
        sl->data[i] = (uint32_t)THEN_WAIT_HSYNC << 16;
    }
    sl->length = LINES_IN_BACK_PORCH;

    vga_submit_scanline(sl);
}

// Set up and return a new display scanline
struct vga_scanline *vga_prepare_scanline() {
    struct vga_scanline *scanline = &scanline_queue[scanline_queue_head];

    // Wait for the scanline buffer to become available again
    while(scanline->_flags & FLAG_BUSY)
        tight_loop_contents();

    // Reinitialize the scanline struct for reuse
    scanline->length = 0;
    scanline->repeat_count = 0;
    scanline->_flags = FLAG_BUSY;
    scanline->_sync = (uint32_t)THEN_WAIT_HSYNC << 16;

    scanline_queue_head = (scanline_queue_head + 1) % NUM_SCANLINE_BUFFERS;

    return scanline;
}

// Mark the scanline as ready so it can be displayed
void vga_submit_scanline(struct vga_scanline *scanline) {
    spin_lock_t *lock = spin_lock_instance(CONFIG_VGA_SPINLOCK_ID);

    scanline->data[scanline->length] = 0; // ensure beam off at end of line

    const uint32_t irq_status = spin_lock_blocking(lock);
    scanline->_flags |= FLAG_READY;
    trigger_ready_scanline_dma();
    spin_unlock(lock, irq_status);
}
