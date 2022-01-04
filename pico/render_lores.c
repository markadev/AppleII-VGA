#include <pico/stdlib.h>
#include "render.h"
#include "vga.h"


#define _RGB333(r, g, b) ( \
    (((((uint)(r) * 256 / 36) + 128) / 256) << 6) | \
    (((((uint)(g) * 256 / 36) + 128) / 256) << 3) | \
    ((((uint)(b) * 256 / 36) + 128) / 256) \
)
static uint16_t lores_palette[16] = {
    _RGB333(0x00,0x00,0x00),    // black
    _RGB333(0x99,0x03,0x5f),    // d.magenta
    _RGB333(0x42,0x04,0xea),    // d.blue
    _RGB333(0xca,0x13,0xfe),    // h.violet
    _RGB333(0x00,0x73,0x10),    // d.green/d.bluegreen
    _RGB333(0x7f,0x7f,0x7f),    // gray.5
    _RGB333(0x24,0x97,0xff),    // h.blue
    _RGB333(0xaa,0xa2,0xff),    // l.blue
    _RGB333(0x4f,0x51,0x01),    // d.brown
    _RGB333(0xf0,0x5c,0x00),    // h.orange
    _RGB333(0x7f,0x7f,0x7f),    // gray.a
    _RGB333(0xff,0x85,0xe1),    // l.magenta
    _RGB333(0x12,0xca,0x07),    // h.green
    _RGB333(0xce,0xd4,0x13),    // l.brown
    _RGB333(0x51,0xf5,0x95),    // l.bluegreen
    _RGB333(0xff,0xff,0xff),    // white
};
#undef _RGB333


static void render_lores_line(uint8_t *page, uint line);


void __time_critical_func(render_lores)(uint8_t *page) {
    vga_prepare_frame();

    // Skip 48 lines to center vertically
    struct vga_scanline *skip_sl = vga_prepare_scanline();
    for(int i=0; i < 48; i++) {
        skip_sl->data[i] = (uint32_t)THEN_WAIT_HSYNC << 16;
    }
    skip_sl->length = 48;
    vga_submit_scanline(skip_sl);

    for(uint line=0; line < 24; line++) {
        render_lores_line(page, line);
    }
}


void __time_critical_func(render_mixed_lores)(uint8_t *page) {
    vga_prepare_frame();

    // Skip 48 lines to center vertically
    struct vga_scanline *skip_sl = vga_prepare_scanline();
    for(int i=0; i < 48; i++) {
        skip_sl->data[i] = (uint32_t)THEN_WAIT_HSYNC << 16;
    }
    skip_sl->length = 48;
    vga_submit_scanline(skip_sl);

    for(uint line=0; line < 20; line++) {
        render_lores_line(page, line);
    }

    for(uint line=20; line < 24; line++) {
        render_text_line(page, line);
    }
}


static void __time_critical_func(render_lores_line)(uint8_t *page, uint line) {
    const uint8_t *line_buf = &page[((line & 0x7) << 7) + (((line >> 3) & 0x3) * 40)];

    // Construct two scanlines for the two different colored cells at the same time
    struct vga_scanline *sl1 = vga_prepare_scanline();
    struct vga_scanline *sl2 = vga_prepare_scanline();
    uint sl_pos = 0;

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
