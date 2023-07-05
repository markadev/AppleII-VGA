#include "render.h"

#include <pico/stdlib.h>
#include "buffers.h"
#include "colors.h"
#include "vga.h"


static void render_lores_line(uint line);
#ifdef APPLE_MODEL_IIE
static void render_dlores_line(uint line);
#endif


// clang-format off
static uint16_t lores_mono_dot_pattern[16] = {
    0x0000, 0x2222, 0x1111, 0x3333, 0x0888, 0x2aaa, 0x1999, 0x3bbb,
    0x0444, 0x2666, 0x1555, 0x3777, 0x0ccc, 0x2eee, 0x1ddd, 0x3fff,
};
// clang-format on


static inline uint __time_critical_func(lores_line_to_mem_offset)(uint line) {
    return ((line & 0x7) << 7) + (((line >> 3) & 0x3) * 40);
}


void __time_critical_func(render_lores)() {
    vga_prepare_frame();
    // Skip 48 lines to center vertically
    vga_skip_lines(48);

#ifdef APPLE_MODEL_IIE
    if(soft_80col && soft_dhires) {
        for(uint line = 0; line < 24; line++) {
            render_dlores_line(line);
        }
    } else
#endif
    {
        for(uint line = 0; line < 24; line++) {
            render_lores_line(line);
        }
    }
}


void __time_critical_func(render_mixed_lores)() {
    vga_prepare_frame();
    // Skip 48 lines to center vertically
    vga_skip_lines(48);

#ifdef APPLE_MODEL_IIE
    if(soft_80col && soft_dhires) {
        for(uint line = 0; line < 20; line++) {
            render_dlores_line(line);
        }

        for(uint line = 20; line < 24; line++) {
            render_text80_line(line);
        }
    } else
#endif
    {
        for(uint line = 0; line < 20; line++) {
            render_lores_line(line);
        }

        for(uint line = 20; line < 24; line++) {
            render_text_line(line);
        }
    }
}


static void __time_critical_func(render_lores_line)(uint line) {
    // Construct two scanlines for the two different colored cells at the same time
    struct vga_scanline *sl1 = vga_prepare_scanline();
    struct vga_scanline *sl2 = vga_prepare_scanline();
    uint sl_pos = 0;

    const uint8_t *page = ((soft_switches & SOFTSW_PAGE_2) && !soft_80store) ? text_p2 : text_p1;
    const uint8_t *line_buf = page + lores_line_to_mem_offset(line);

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
        for(int i = 0; i < 40; i += 2) {
            uint32_t color1 = lores_mono_dot_pattern[line_buf[i] & 0xf] << 14;
            uint32_t color2 = lores_mono_dot_pattern[(line_buf[i] >> 4) & 0xf] << 14;
            color1 |= lores_mono_dot_pattern[line_buf[i + 1] & 0xf];
            color2 |= lores_mono_dot_pattern[(line_buf[i + 1] >> 4) & 0xf];

            for(int j = 0; j < 14; j++) {
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

    sl1->length = sl_pos;
    sl1->repeat_count = 7;
    vga_submit_scanline(sl1);

    sl2->length = sl_pos;
    sl2->repeat_count = 7;
    vga_submit_scanline(sl2);
}


#ifdef APPLE_MODEL_IIE
static void __time_critical_func(render_dlores_line)(uint line) {
    // Construct two scanlines for the two different colored cells at the same time
    struct vga_scanline *sl1 = vga_prepare_scanline();
    struct vga_scanline *sl2 = vga_prepare_scanline();
    uint sl_pos = 0;

    const uint8_t *page_main = ((soft_switches & SOFTSW_PAGE_2) && !soft_80store) ? text_p2 : text_p1;
    const uint8_t *page_aux = ((soft_switches & SOFTSW_PAGE_2) && !soft_80store) ? text_p4 : text_p3;
    const uint line_offset = lores_line_to_mem_offset(line);
    const uint8_t *line_main = page_main + line_offset;
    const uint8_t *line_aux = page_aux + line_offset;

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

    for(int i = 0; i < 40; i++) {
        // First pixel data is from aux memory
        uint32_t color_aux1 = dhgr_palette[line_aux[i] & 0xf];
        uint32_t color_aux2 = dhgr_palette[(line_aux[i] >> 4) & 0xf];

        // Next pixel data is from main memory
        uint32_t color_main1 = lores_palette[line_main[i] & 0xf];
        uint32_t color_main2 = lores_palette[(line_main[i] >> 4) & 0xf];

        // Each double-lores pixel is 3.5 hires pixels, or 7 VGA pixels wide
        sl1->data[sl_pos] = (color_aux1 | THEN_EXTEND_6) | ((color_main1 | THEN_EXTEND_6) << 16);
        sl2->data[sl_pos] = (color_aux2 | THEN_EXTEND_6) | ((color_main2 | THEN_EXTEND_6) << 16);
        sl_pos++;
    }

    sl1->length = sl_pos;
    sl1->repeat_count = 7;
    vga_submit_scanline(sl1);

    sl2->length = sl_pos;
    sl2->repeat_count = 7;
    vga_submit_scanline(sl2);
}
#endif
