#pragma once

#include <stdint.h>


#define VGA_WIDTH 640
#define VGA_HEIGHT 480


#define THEN_WAIT_VSYNC (2 << 9)
#define THEN_WAIT_HSYNC (3 << 9)
#define THEN_EXTEND_1   (6 << 9)
#define THEN_EXTEND_2   (5 << 9)
#define THEN_EXTEND_3   (4 << 9)


struct vga_scanline {
    // number of 32-bit words in the data array
    uint_fast16_t length;

    // number of times to repeat the scanline
    uint_fast16_t repeat_count;

    volatile uint_fast8_t _flags;

    uint32_t _sync;
    uint32_t data[(VGA_WIDTH/2)+1];
};


void vga_init();

struct vga_scanline *vga_prepare_scanline(bool vsync);
void vga_submit_scanline(struct vga_scanline *scanline);
