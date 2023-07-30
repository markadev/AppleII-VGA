#include "render.h"

#include <pico/stdlib.h>
#include "buffers.h"
#include "colors.h"
#include "vga.h"


static void render_lores_line(uint line);
#ifdef APPLE_MODEL_IIE
static void render_dlores_line(uint line);
#endif


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
    } else
#endif
    {
        for(uint line = 0; line < 20; line++) {
            render_lores_line(line);
        }
    }

    for(uint line = 20; line < 24; line++) {
        render_text_line(line);
    }
}


static void __time_critical_func(render_lores_line)(uint line) {
    // Construct two scanlines for the two different colored cells at the same time
    struct vga_scanline *sl1 = vga_prepare_scanline();
    struct vga_scanline *sl2 = vga_prepare_scanline();
    uint sl_pos = 0;

    const uint8_t *page = is_page2_display_enabled() ? text_mainmem_page2 : text_mainmem_page1;
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
        const uint32_t bits_to_pixels[4] = {
            ((uint32_t)mono_bg_color << 16) | mono_bg_color,
            ((uint32_t)mono_bg_color << 16) | mono_fg_color,
            ((uint32_t)mono_fg_color << 16) | mono_bg_color,
            ((uint32_t)mono_fg_color << 16) | mono_fg_color,
        };

        for(int i = 0; i < 40; i += 2) {
            // Construct the bit patterns for both lines of the next two lores columns by extending each
            // pixel's 4-bit color pattern to 14 bits
            uint32_t bitpattern1 = (uint32_t)(line_buf[i] & 0xf) | ((uint32_t)(line_buf[i + 1] & 0xf) << 14);
            uint32_t bitpattern2 = (uint32_t)((line_buf[i] >> 4) & 0xf) | ((uint32_t)((line_buf[i + 1] >> 4) & 0xf) << 14);

            bitpattern1 |= (bitpattern1 << 4);
            bitpattern1 |= (bitpattern1 & ~(uint32_t)0xc0) << 8;
            bitpattern2 |= (bitpattern2 << 4);
            bitpattern2 |= (bitpattern2 & ~(uint32_t)0xc0) << 8;

            // Render out the pixels, least significant bit first
            for(int j = 0; j < 14; j++) {
                sl1->data[sl_pos] = bits_to_pixels[bitpattern1 & 0x03];
                sl2->data[sl_pos] = bits_to_pixels[bitpattern2 & 0x03];
                sl_pos++;

                bitpattern1 >>= 2;
                bitpattern2 >>= 2;
            }
        }
    } else {
        for(int i = 0; i < 40; i++) {
            uint32_t color1 = ntsc_palette[line_buf[i] & 0xf];
            uint32_t color2 = ntsc_palette[(line_buf[i] >> 4) & 0xf];

            // Each lores pixel is 7 hires pixels, or 14 VGA pixels wide
            sl1->data[sl_pos] = (color1 | THEN_EXTEND_6) | ((color1 | THEN_EXTEND_6) << 16);
            sl2->data[sl_pos] = (color2 | THEN_EXTEND_6) | ((color2 | THEN_EXTEND_6) << 16);
            sl_pos++;
        }
    }

    if(soft_scanline_emulation) {
        // Just insert a blank scanline between each rendered scanline
        sl1->data[sl_pos] = THEN_WAIT_HSYNC;
        sl2->data[sl_pos] = THEN_WAIT_HSYNC;
        sl_pos++;

        sl1->repeat_count = 3;
        sl2->repeat_count = 3;
    } else {
        sl1->repeat_count = 7;
        sl2->repeat_count = 7;
    }

    sl1->length = sl_pos;
    sl2->length = sl_pos;
    vga_submit_scanline(sl1);
    vga_submit_scanline(sl2);
}


#ifdef APPLE_MODEL_IIE
static void __time_critical_func(render_dlores_line)(uint line) {
    // Construct two scanlines for the two different colored cells at the same time
    struct vga_scanline *sl1 = vga_prepare_scanline();
    struct vga_scanline *sl2 = vga_prepare_scanline();
    uint sl_pos = 0;

    const uint8_t *page_main = is_page2_display_enabled() ? text_mainmem_page2 : text_mainmem_page1;
    const uint8_t *page_aux = is_page2_display_enabled() ? text_auxmem_page2 : text_auxmem_page1;
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

    if(soft_monochrom) {
        const uint32_t bits_to_pixels[4] = {
            ((uint32_t)mono_bg_color << 16) | mono_bg_color,
            ((uint32_t)mono_bg_color << 16) | mono_fg_color,
            ((uint32_t)mono_fg_color << 16) | mono_bg_color,
            ((uint32_t)mono_fg_color << 16) | mono_fg_color,
        };

        for(int i = 0; i < 40; i++) {
            // Construct the bit patterns for both lines of the next two lores columns by extending each
            // pixel's 4-bit color pattern to 7 bits
            uint32_t bitpattern1 = (uint32_t)(line_aux[i] & 0xf) | ((uint32_t)(line_main[i] & 0xf) << 7);
            uint32_t bitpattern2 = (uint32_t)((line_aux[i] >> 4) & 0xf) | ((uint32_t)((line_main[i] >> 4) & 0xf) << 7);

            bitpattern1 |= ((bitpattern1 & ~(uint32_t)0x7) << 4);
            bitpattern2 |= ((bitpattern2 & ~(uint32_t)0x7) << 4);

            // Render out the next 14 pixels, least significant bit first
            for(int j = 0; j < 7; j++) {
                sl1->data[sl_pos] = bits_to_pixels[bitpattern1 & 0x03];
                sl2->data[sl_pos] = bits_to_pixels[bitpattern2 & 0x03];
                sl_pos++;

                bitpattern1 >>= 2;
                bitpattern2 >>= 2;
            }
        }
    } else {
        for(int i = 0; i < 40; i++) {
            // First pixel data is from aux memory
            uint32_t color_aux1 = ntsc90_palette[line_aux[i] & 0xf];
            uint32_t color_aux2 = ntsc90_palette[(line_aux[i] >> 4) & 0xf];

            // Next pixel data is from main memory
            uint32_t color_main1 = ntsc_palette[line_main[i] & 0xf];
            uint32_t color_main2 = ntsc_palette[(line_main[i] >> 4) & 0xf];

            // Each double-lores pixel is 3.5 hires pixels, or 7 VGA pixels wide
            sl1->data[sl_pos] = (color_aux1 | THEN_EXTEND_6) | ((color_main1 | THEN_EXTEND_6) << 16);
            sl2->data[sl_pos] = (color_aux2 | THEN_EXTEND_6) | ((color_main2 | THEN_EXTEND_6) << 16);
            sl_pos++;
        }
    }

    if(soft_scanline_emulation) {
        // Just insert a blank scanline between each rendered scanline
        sl1->data[sl_pos] = THEN_WAIT_HSYNC;
        sl2->data[sl_pos] = THEN_WAIT_HSYNC;
        sl_pos++;

        sl1->repeat_count = 3;
        sl2->repeat_count = 3;
    } else {
        sl1->repeat_count = 7;
        sl2->repeat_count = 7;
    }

    sl1->length = sl_pos;
    sl2->length = sl_pos;
    vga_submit_scanline(sl1);
    vga_submit_scanline(sl2);
}
#endif
