#include <pico/stdlib.h>
#include "buffers.h"
#include "render.h"
#include "vga.h"


#define _RGB333(r, g, b) ( \
    (((((uint)(r) * 256 / 36) + 128) / 256) << 6) | \
    (((((uint)(g) * 256 / 36) + 128) / 256) << 3) | \
    ((((uint)(b) * 256 / 36) + 128) / 256) \
)
static uint16_t lores_palette[16] = {
    _RGB333(0x00,0x00,0x00),    // black
    _RGB333(0x6c,0x00,0x6c),    // magenta
    _RGB333(0x00,0x00,0xb4),    // d.blue
    _RGB333(0xb4,0x24,0xfc),    // h.violet
    _RGB333(0x00,0x48,0x00),    // d.green
    _RGB333(0x48,0x48,0x48),    // d.gray
    _RGB333(0x00,0x90,0xfc),    // h.blue
    _RGB333(0x6c,0x6c,0xfc),    // l.blue
    _RGB333(0x24,0x24,0x00),    // brown
    _RGB333(0xfc,0x48,0x00),    // h.orange
    _RGB333(0x90,0x90,0x90),    // l.gray
    _RGB333(0xfc,0x6c,0xfc),    // pink
    _RGB333(0x00,0xd8,0x24),    // h.green
    _RGB333(0xd8,0xd8,0x00),    // yellow
    _RGB333(0x90,0xfc,0xb4),    // aqua
    _RGB333(0xff,0xff,0xff),    // white
};
#undef _RGB333


static void render_lores_line(uint line);


void __time_critical_func(render_lores)() {
    vga_prepare_frame();

    // Skip 48 lines to center vertically
    struct vga_scanline *skip_sl = vga_prepare_scanline();
    for(int i=0; i < 48; i++) {
        skip_sl->data[i] = (uint32_t)THEN_WAIT_HSYNC << 16;
    }
    skip_sl->length = 48;
    vga_submit_scanline(skip_sl);

    for(uint line=0; line < 24; line++) {
        render_lores_line(line);
    }
}


void __time_critical_func(render_mixed_lores)() {
    vga_prepare_frame();

    // Skip 48 lines to center vertically
    struct vga_scanline *skip_sl = vga_prepare_scanline();
    for(int i=0; i < 48; i++) {
        skip_sl->data[i] = (uint32_t)THEN_WAIT_HSYNC << 16;
    }
    skip_sl->length = 48;
    vga_submit_scanline(skip_sl);

    for(uint line=0; line < 20; line++) {
        render_lores_line(line);
    }

    for(uint line=20; line < 24; line++) {
        render_text_line(line);
    }
}


static void __time_critical_func(render_lores_line)(uint line) {
    // Construct two scanlines for the two different colored cells at the same time
    struct vga_scanline *sl1 = vga_prepare_scanline();
    struct vga_scanline *sl2 = vga_prepare_scanline();
    uint sl_pos = 0;

    const uint8_t *page = (soft_switches & SOFTSW_PAGE_2) ? text_memory + 1024 : text_memory;
    const uint8_t *line_buf = page + ((line & 0x7) << 7) + (((line >> 3) & 0x3) * 40);

    // Pad 40 pixels on the left to center horizontally
    while(sl_pos < 40/8) {
        sl1->data[sl_pos] = (0|THEN_EXTEND_3) | ((0|THEN_EXTEND_3) << 16); // 8 pixels per word
        sl2->data[sl_pos] = (0|THEN_EXTEND_3) | ((0|THEN_EXTEND_3) << 16); // 8 pixels per word
        sl_pos++;
    }

    for(int i=0; i < 40; i++) {
        uint32_t color1 = lores_palette[line_buf[i] & 0xf];
        uint32_t color2 = lores_palette[(line_buf[i] >> 4) & 0xf];

        // Each lores pixel is 7 hires pixels, or 14 VGA pixels wide
        sl1->data[sl_pos] = (color1|THEN_EXTEND_2) | ((color1|THEN_EXTEND_3) << 16);
        sl2->data[sl_pos] = (color2|THEN_EXTEND_2) | ((color2|THEN_EXTEND_3) << 16);
        sl_pos++;
        sl1->data[sl_pos] = (color1|THEN_EXTEND_2) | ((color1|THEN_EXTEND_3) << 16);
        sl2->data[sl_pos] = (color2|THEN_EXTEND_2) | ((color2|THEN_EXTEND_3) << 16);
        sl_pos++;
    }

    sl1->length = sl_pos;
    sl1->repeat_count = 7;
    vga_submit_scanline(sl1);

    sl2->length = sl_pos;
    sl2->repeat_count = 7;
    vga_submit_scanline(sl2);
}
