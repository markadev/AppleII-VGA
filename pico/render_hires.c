#include <pico/stdlib.h>
#include "render.h"
#include "vga.h"


static void render_hires_line(uint8_t *page, uint line);


void __time_critical_func(render_hires)(uint8_t *page) {
    vga_prepare_frame();

    // Skip 48 lines to center vertically
    struct vga_scanline *skip_sl = vga_prepare_scanline();
    for(int i=0; i < 48; i++) {
        skip_sl->data[i] = (uint32_t)THEN_WAIT_HSYNC << 16;
    }
    skip_sl->length = 48;
    vga_submit_scanline(skip_sl);

    for(uint line=0; line < 192; line++) {
        render_hires_line(page, line);
    }
}


void __time_critical_func(render_mixed_hires)(uint8_t *hires_page, uint8_t *text_page) {
    vga_prepare_frame();

    // Skip 48 lines to center vertically
    struct vga_scanline *skip_sl = vga_prepare_scanline();
    for(int i=0; i < 48; i++) {
        skip_sl->data[i] = (uint32_t)THEN_WAIT_HSYNC << 16;
    }
    skip_sl->length = 48;
    vga_submit_scanline(skip_sl);

    for(uint line=0; line < 160; line++) {
        render_hires_line(hires_page, line);
    }

    for(uint line=20; line < 24; line++) {
        render_text_line(text_page, line);
    }
}


static void __time_critical_func(render_hires_line)(uint8_t *page, uint line) {
    struct vga_scanline *sl = vga_prepare_scanline();

    // TODO

    sl->repeat_count = 1;
    vga_submit_scanline(sl);
}
