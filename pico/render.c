#include <pico/stdlib.h>
#include <hardware/timer.h>
#include "buffers.h"
#include "render.h"
#include "vga.h"


static void __noinline __time_critical_func(render_testpattern)() {
    vga_prepare_frame();

    for(uint line=0; line < VGA_HEIGHT; line++) {
        struct vga_scanline *sl = vga_prepare_scanline();

        if(line == 0 || line == VGA_HEIGHT-1) {
            for(uint i=0; i < VGA_WIDTH/4; i++) {
                sl->data[i] = ((uint32_t)0x1ff | THEN_EXTEND_1) | (((uint32_t)0x1ff | THEN_EXTEND_1) << 16);
            }
            sl->length = VGA_WIDTH/4; // number of 32-bit words
        } else if(line < 120) {
            for(uint32_t i=0; i < 256; i+=2) {
                uint32_t w = ((i & 0x1ff) | THEN_EXTEND_1) |
                    (((i+1 & 0x1ff) | THEN_EXTEND_1) << 16);
                sl->data[i/2] = w;
            }
            sl->length = 256/2;
        } else if(line < 140) {
            for(uint i=0; i < VGA_WIDTH/4; i++) {
                sl->data[i] = (uint32_t)0x1ff;
            }
            for(uint i=VGA_WIDTH/4; i < VGA_WIDTH/2; i++) {
                sl->data[i] = (uint32_t)0x1ff << 16;
            }
            sl->length = VGA_WIDTH/2;
            sl->repeat_count = 19;
            line += 19;
        }

        vga_submit_scanline(sl);
    }
}


static void __noinline __time_critical_func(render_hires)(uint8_t *page) {
    // TODO
    render_testpattern();
}


static void __noinline __time_critical_func(render_mixed_hires)(uint8_t *hires_page, uint8_t *text_page) {
    // TODO
    render_testpattern();
}


void render_loop() {
    while(1) {
        update_text_flasher();

        switch(soft_switches & SOFTSW_MODE_MASK) {
        case 0:
            render_lores((soft_switches & SOFTSW_PAGE_2) ? text_memory + 1024 : text_memory);
            break;
        case SOFTSW_MIX_MODE:
            render_mixed_lores((soft_switches & SOFTSW_PAGE_2) ? text_memory + 1024 : text_memory);
            break;
        case SOFTSW_HIRES_MODE:
            render_hires((soft_switches & SOFTSW_PAGE_2) ? hires_memory + 8192 : hires_memory);
            break;
        case SOFTSW_HIRES_MODE|SOFTSW_MIX_MODE:
            render_mixed_hires(
                (soft_switches & SOFTSW_PAGE_2) ? hires_memory + 8192 : hires_memory,
                (soft_switches & SOFTSW_PAGE_2) ? text_memory + 1024 : text_memory);
            break;
        default:
            render_text((soft_switches & SOFTSW_PAGE_2) ? text_memory + 1024 : text_memory);
            break;
        }
    }
}
