#include "render.h"

#include <pico/stdlib.h>
#include "buffers.h"
#include "colors.h"
#include "lores_dot_patterns.h"
#include "vga.h"


static void render_lores_line(uint line);
#ifdef APPLE_MODEL_IIE
static void drender_lores_line(uint line);
#endif


void __time_critical_func(render_lores)() {
    vga_prepare_frame();

    // Skip 48 lines to center vertically
    struct vga_scanline *skip_sl = vga_prepare_scanline();
    for(int i = 0; i < 48; i++) {
        skip_sl->data[i] = (uint32_t)THEN_WAIT_HSYNC << 16;
    }
    skip_sl->length = 48;
    vga_submit_scanline(skip_sl);

    for(uint line = 0; line < 24; line++) {
#ifdef APPLE_MODEL_IIE
        if(soft_dhires) {
            drender_lores_line(line);
        } else {
            render_lores_line(line);
        }
#else
        render_lores_line(line);
#endif
    }
}


void __time_critical_func(render_mixed_lores)() {
    vga_prepare_frame();

    // Skip 48 lines to center vertically
    struct vga_scanline *skip_sl = vga_prepare_scanline();
    for(int i = 0; i < 48; i++) {
        skip_sl->data[i] = (uint32_t)THEN_WAIT_HSYNC << 16;
    }
    skip_sl->length = 48;
    vga_submit_scanline(skip_sl);

    for(uint line = 0; line < 20; line++) {
#ifdef APPLE_MODEL_IIE
        if(soft_dhires) {
            drender_lores_line(line);
        } else {
            render_lores_line(line);
        }
#else
        render_lores_line(line);
#endif
    }
#ifdef APPLE_MODEL_IIE
    if(soft_80col || soft_dhires) {
        for(uint line = 20; line < 24; line++) {
            render_text80_line(line);
        }
    } else {
        for(uint line = 20; line < 24; line++) {
            render_text_line(line);
        }
    }
#else
    for(uint line = 20; line < 24; line++) {
        render_text_line(line);
    }
#endif
}


static void __time_critical_func(render_lores_line)(uint line) {
    // Construct two scanlines for the two different colored cells at the same time
    struct vga_scanline *sl1 = vga_prepare_scanline();
    struct vga_scanline *sl2 = vga_prepare_scanline();
    uint sl_pos = 0;
    uint i, j;
    uint32_t color1, color2;

    const uint8_t *page = (const uint8_t *)(((soft_switches & SOFTSW_PAGE_2) && !soft_80store) ? text_p2 : text_p1);
    const uint8_t *line_buf = (const uint8_t *)(page + ((line & 0x7) << 7) + (((line >> 3) & 0x3) * 40));

    // Pad 40 pixels on the left to center horizontally
    sl1->data[sl_pos] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl2->data[sl_pos] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl_pos++;
    sl1->data[sl_pos] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl2->data[sl_pos] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl_pos++;
    sl1->data[sl_pos] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels per word
    sl2->data[sl_pos] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels per word
    sl_pos++;

    if(soft_monochrom) {
        for(i = 0; i < 40; i += 2) {
            color1 = lores_dot_pattern[line_buf[i] & 0xf] << 14;
            color2 = lores_dot_pattern[(line_buf[i] >> 4) & 0xf] << 14;
            color1 |= lores_dot_pattern[line_buf[i + 1] & 0xf];
            color2 |= lores_dot_pattern[(line_buf[i + 1] >> 4) & 0xf];

            for(j = 0; j < 14; j++) {
                uint32_t pixeldata;

                pixeldata = (color1 & 0x8000000) ? (lores_palette[15]) : (lores_palette[0]);
                pixeldata |= (color1 & 0x4000000) ? ((lores_palette[15]) << 16) : ((lores_palette[0]) << 16);
                color1 <<= 2;
                sl1->data[sl_pos] = pixeldata;

                pixeldata = (color2 & 0x8000000) ? (lores_palette[15]) : (lores_palette[0]);
                pixeldata |= (color2 & 0x4000000) ? ((lores_palette[15]) << 16) : ((lores_palette[0]) << 16);
                sl2->data[sl_pos] = pixeldata;
                color2 <<= 2;

                sl_pos++;
            }
        }
    } else {
        for(int i = 0; i < 40; i++) {
            uint32_t color1 = lores_palette[line_buf[i] & 0xf];
            uint32_t color2 = lores_palette[(line_buf[i] >> 4) & 0xf];

            // Each lores pixel is 7 hires pixels, or 14 VGA pixels wide
            sl1->data[sl_pos] = (color1 | THEN_EXTEND_6) | ((color1 | THEN_EXTEND_6) << 16);
            sl2->data[sl_pos] = (color2 | THEN_EXTEND_6) | ((color2 | THEN_EXTEND_6) << 16);
            sl_pos++;
        }
    }
    sl1->data[sl_pos] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl2->data[sl_pos] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl_pos++;
    sl1->data[sl_pos] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl2->data[sl_pos] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl_pos++;
    sl1->data[sl_pos] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels per word
    sl2->data[sl_pos] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels per word
    sl_pos++;
    sl1->length = sl_pos;
    sl1->repeat_count = 7;
    vga_submit_scanline(sl1);

    sl2->length = sl_pos;
    sl2->repeat_count = 7;
    vga_submit_scanline(sl2);
}


#ifdef APPLE_MODEL_IIE
static void __time_critical_func(drender_lores_line)(uint line) {
    // Construct two scanlines for the two different colored cells at the same time
    struct vga_scanline *sl1 = vga_prepare_scanline();
    struct vga_scanline *sl2 = vga_prepare_scanline();
    uint sl_pos = 0;
    uint i;
    uint32_t color1, color2;
    uint_fast8_t dotc = 0;
    uint32_t pixeldata;

    const uint8_t *line_bufa = (const uint8_t *)((((soft_switches & SOFTSW_PAGE_2) && !soft_80store) ? text_p2 : text_p1) +
                                                 ((line & 0x7) << 7) + (((line >> 3) & 0x3) * 40));

    const uint8_t *line_bufb = (const uint8_t *)((((soft_switches & SOFTSW_PAGE_2) && !soft_80store) ? text_p4 : text_p3) +
                                                 ((line & 0x7) << 7) + (((line >> 3) & 0x3) * 40));

    // Pad 40 pixels on the left to center horizontally
    sl1->data[sl_pos] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl2->data[sl_pos] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl_pos++;
    sl1->data[sl_pos] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl2->data[sl_pos] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl_pos++;
    sl1->data[sl_pos] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels per word
    sl2->data[sl_pos] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels per word
    sl_pos++;

    i = 0;
    color1 = 0;
    color2 = 0;

    while(i < 40) {
        while((dotc <= 14) && (i < 40)) {
            color1 |= dgr_dot_pattern[((i & 1) << 4) | (line_bufb[i] & 0xf)] << dotc;
            color2 |= dgr_dot_pattern[((i & 1) << 4) | ((line_bufb[i] >> 4) & 0xf)] << dotc;
            dotc += 7;
            color1 |= dgr_dot_pattern[((i & 1) << 4) | (line_bufa[i] & 0xf)] << dotc;
            color2 |= dgr_dot_pattern[((i & 1) << 4) | ((line_bufa[i] >> 4) & 0xf)] << dotc;
            dotc += 7;
            i++;
        }

        // Consume pixels
        while((dotc >= 8) || ((dotc > 0) && (i == 40))) {
            color1 &= 0xfffffffe;
            color1 |= (color1 >> 4) & 1;
            pixeldata = dhgr_palette[color1 & 0xf];
            color1 &= 0xfffffffc;
            color1 |= (color1 >> 4) & 3;
            pixeldata |= dhgr_palette[color1 & 0xf] << 16;
            sl1->data[sl_pos] = pixeldata;

            color2 &= 0xfffffffe;
            color2 |= (color2 >> 4) & 1;
            pixeldata = dhgr_palette[color2 & 0xf];
            color2 &= 0xfffffffc;
            color2 |= (color2 >> 4) & 3;
            pixeldata |= dhgr_palette[color2 & 0xf] << 16;
            sl2->data[sl_pos] = pixeldata;

            sl_pos++;

            color1 &= 0xfffffff8;
            color1 |= (color1 >> 4) & 7;
            pixeldata = dhgr_palette[color1 & 0xf];
            color1 >>= 4;
            pixeldata |= dhgr_palette[color1 & 0xf] << 16;
            sl1->data[sl_pos] = pixeldata;

            color2 &= 0xfffffff8;
            color2 |= (color2 >> 4) & 7;
            pixeldata = dhgr_palette[color2 & 0xf];
            color2 >>= 4;
            pixeldata |= dhgr_palette[color2 & 0xf] << 16;
            sl2->data[sl_pos] = pixeldata;

            sl_pos++;
            dotc -= 4;
        }
    }

    // Pad 40 pixels on the right to center horizontally
    sl1->data[sl_pos] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl2->data[sl_pos] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl_pos++;
    sl1->data[sl_pos] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl2->data[sl_pos] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl_pos++;
    sl1->data[sl_pos] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels per word
    sl2->data[sl_pos] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels per word
    sl_pos++;

    sl1->length = sl_pos;
    sl1->repeat_count = 7;
    vga_submit_scanline(sl1);

    sl2->length = sl_pos;
    sl2->repeat_count = 7;
    vga_submit_scanline(sl2);
}
#endif
