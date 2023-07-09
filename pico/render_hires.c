#include "render.h"

#include <pico/stdlib.h>
#include "buffers.h"
#include "colors.h"
#include "hires_color_patterns.h"
#include "hires_dot_patterns.h"
#include "vga.h"


static void render_hires_line(uint line);
#ifdef APPLE_MODEL_IIE
static void render_dhires_line(uint line);
#endif


static inline uint hires_line_to_mem_offset(uint line) {
    return ((line & 0x07) << 10) | ((line & 0x38) << 4) | (((line & 0xc0) >> 6) * 40);
}


void __time_critical_func(render_hires)() {
    vga_prepare_frame();
    // Skip 48 lines to center vertically
    vga_skip_lines(48);

    for(uint line = 0; line < 192; line++) {
#ifdef APPLE_MODEL_IIE
        if(soft_dhires) {
            render_dhires_line(line);
        } else {
            render_hires_line(line);
        }
#else
        render_hires_line(line);
#endif
    }
}


void __time_critical_func(render_mixed_hires)() {
    vga_prepare_frame();
    // Skip 48 lines to center vertically
    vga_skip_lines(48);

    for(uint line = 0; line < 160; line++) {
#ifdef APPLE_MODEL_IIE
        if(soft_dhires) {
            render_dhires_line(line);
        } else {
            render_hires_line(line);
        }
#else
        render_hires_line(line);
#endif
    }

    for(uint line = 20; line < 24; line++) {
        render_text_line(line);
    }
}


static void __time_critical_func(render_hires_line)(uint line) {
    struct vga_scanline *sl = vga_prepare_scanline();
    uint sl_pos = 0;

    const uint8_t *page = (const uint8_t *)(((soft_switches & SOFTSW_PAGE_2) && !soft_80store) ? hgr_p2 : hgr_p1);
    const uint8_t *line_mem = (const uint8_t *)(page + hires_line_to_mem_offset(line));

    // Pad 40 pixels on the left to center horizontally
    sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels
    sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels
    sl->data[sl_pos++] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels

    // Each hires byte contains 7 pixels which may be shifted right 1/2 a pixel.
    // That is represented here by 14 'dots' to precisely describe the half-pixel
    // positioning.
    //
    // For each pixel, inspect a window of 8 dots around the pixel to determine the
    // precise dot locations and colors.
    //
    // Dots would be scanned out to the CRT from MSB to LSB (left to right here):
    //
    //            previous   |        next
    //              dots     |        dots
    //        +-------------------+--------------------------------------------------+
    // dots:  | 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 | 23 | ... | 14 | 13 | 12 | ...
    //        |              |         |              |
    //        \______________|_________|______________/
    //                       |         |
    //                       \_________/
    //                         current
    //                          pixel
    uint32_t dots = 0;
    uint oddness = 0;

    // Load in the first 14 dots
    dots |= (uint32_t)hires_dot_patterns[line_mem[0]] << 15;

    for(uint i = 1; i < 41; i++) {
        // Load in the next 14 dots
        uint b = (i < 40) ? line_mem[i] : 0;
        if(b & 0x80) {
            // Extend the last bit from the previous byte
            dots |= (dots & (1u << 15)) >> 1;
        }
        dots |= (uint32_t)hires_dot_patterns[b] << 1;

        // Consume 14 dots
        for(uint j = 0; j < 7; j++) {
            uint dot_pattern = oddness | ((dots >> 24) & 0xff);
            sl->data[sl_pos] = hires_color_patterns[dot_pattern];
            sl_pos++;
            dots <<= 2;
            oddness ^= 0x100;
        }
    }

    sl->length = sl_pos;
    sl->repeat_count = 1;
    vga_submit_scanline(sl);
}


#ifdef APPLE_MODEL_IIE
static void __time_critical_func(render_dhires_line)(uint line) {
    uint sl_pos = 0;
    uint i, j;
    uint_fast8_t dotc = 0;
    uint32_t dots = 0;
    uint32_t color1, color2, color3, color4;
    uint32_t pixeldata;
    uint32_t pixelmode = 0;

    const uint8_t *page = (const uint8_t *)(((soft_switches & SOFTSW_PAGE_2) && !soft_80store) ? hgr_p2 : hgr_p1);
    const uint8_t *aux_page = (const uint8_t *)(((soft_switches & SOFTSW_PAGE_2) && !soft_80store) ? hgr_p4 : hgr_p3);
    const uint8_t *line_mema = (const uint8_t *)(page + hires_line_to_mem_offset(line));
    const uint8_t *line_memb = (const uint8_t *)(aux_page + hires_line_to_mem_offset(line));
    struct vga_scanline *sl = vga_prepare_scanline();

    // Pad 40 pixels on the left to center horizontally
    if(soft_video7_mode != VIDEO7_MODE_160x192) {
        sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels
        sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels
        sl->data[sl_pos++] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels
    }
    i = 0;

    // Video 7 F/B HiRes
    if(soft_monochrom) {
        while(i < 40) {
            // Load in as many subpixels as possible
            while((dotc < 28) && (i < 40)) {
                dots |= (line_memb[i] & 0x7f) << dotc;
                dotc += 7;
                dots |= (line_mema[i] & 0x7f) << dotc;
                dotc += 7;
                i++;
            }

            // Consume pixels
            while(dotc) {
                pixeldata = ((dots & 1) ? (dhgr_palette[15]) : (dhgr_palette[0]));
                dots >>= 1;
                pixeldata |= (((dots & 1) ? (dhgr_palette[15]) : (dhgr_palette[0]))) << 16;
                dots >>= 1;
                sl->data[sl_pos++] = pixeldata;
                dotc -= 2;
            }
        }
    } else if(soft_80store && !soft_80col) {
        // Video 7 F/B HiRes
        while(i < 40) {
            dots = (line_mema[i] & 0x7f);
            color1 = lores_palette[(line_memb[i] >> 4) & 0xF];
            color2 = lores_palette[(line_memb[i] >> 0) & 0xF];
            i++;

            dots |= (line_mema[i] & 0x7f) << 7;
            color3 = lores_palette[(line_memb[i] >> 4) & 0xF];
            color4 = lores_palette[(line_memb[i] >> 0) & 0xF];
            i++;

            for(j = 0; j < 3; j++) {
                pixeldata = ((dots & 1) ? (color1) : (color2)) | THEN_EXTEND_1;
                dots >>= 1;
                pixeldata |= (((dots & 1) ? (color1) : (color2)) | THEN_EXTEND_1) << 16;
                dots >>= 1;
                sl->data[sl_pos++] = pixeldata;
            }

            pixeldata = ((dots & 1) ? (color1) : (color2)) | THEN_EXTEND_1;
            dots >>= 1;
            pixeldata |= (((dots & 1) ? (color3) : (color4)) | THEN_EXTEND_1) << 16;
            dots >>= 1;
            sl->data[sl_pos++] = pixeldata;

            for(j = 0; j < 3; j++) {
                pixeldata = ((dots & 1) ? (color3) : (color4)) | THEN_EXTEND_1;
                dots >>= 1;
                pixeldata |= (((dots & 1) ? (color3) : (color4)) | THEN_EXTEND_1) << 16;
                dots >>= 1;
                sl->data[sl_pos++] = pixeldata;
            }
        }
    } else if(soft_video7_mode == VIDEO7_MODE_160x192) {
        while(i < 40) {
            // Load in as many subpixels as possible
            while((dotc <= 18) && (i < 40)) {
                dots |= (line_memb[i] & 0xff) << dotc;
                dotc += 7;
                dots |= (line_mema[i] & 0xff) << dotc;
                dotc += 7;
                i++;
            }

            // Consume pixels
            while(dotc >= 8) {
                pixeldata = (lores_palette[dots & 0xf] | THEN_EXTEND_3);
                dots >>= 4;
                pixeldata |= (lores_palette[dots & 0xf] | THEN_EXTEND_3) << 16;
                dots >>= 4;
                sl->data[sl_pos++] = pixeldata;
                dotc -= 8;
            }
        }
    } else if(soft_video7_mode == VIDEO7_MODE_MIX) {
        while(i < 40) {
            // Load in as many subpixels as possible
            while((dotc <= 18) && (i < 40)) {
                dots |= (line_memb[i] & 0x7f) << dotc;
                pixelmode |= ((line_memb[i] & 0x80) ? 0x7f : 0x00) << dotc;
                dotc += 7;
                dots |= (line_mema[i] & 0x7f) << dotc;
                pixelmode |= ((line_mema[i] & 0x80) ? 0x7f : 0x00) << dotc;
                dotc += 7;
                i++;
            }

            // Consume pixels
            while(dotc >= 4) {
                if(pixelmode) {
                    pixeldata = (dhgr_palette[dots & 0xf] | THEN_EXTEND_1);
                    pixeldata |= pixeldata << 16;
                    dots >>= 4;
                    pixelmode >>= 4;
                    sl->data[sl_pos++] = pixeldata;
                    dotc -= 4;
                } else {
                    pixeldata = ((dots & 1) ? (dhgr_palette[15]) : (dhgr_palette[0]));
                    dots >>= 1;
                    pixelmode >>= 1;
                    pixeldata |= (((dots & 1) ? (dhgr_palette[15]) : (dhgr_palette[0]))) << 16;
                    dots >>= 1;
                    pixelmode >>= 1;
                    sl->data[sl_pos++] = pixeldata;
                    dotc -= 2;
                    pixeldata = ((dots & 1) ? (dhgr_palette[15]) : (dhgr_palette[0]));
                    dots >>= 1;
                    pixelmode >>= 1;
                    pixeldata |= (((dots & 1) ? (dhgr_palette[15]) : (dhgr_palette[0]))) << 16;
                    dots >>= 1;
                    pixelmode >>= 1;
                    sl->data[sl_pos++] = pixeldata;
                    dotc -= 2;
                }
            }
        }
    } else {
        while(i < 40) {
            // Load in as many subpixels as possible
            while((dotc <= 18) && (i < 40)) {
                dots |= (line_memb[i] & 0x7f) << dotc;
                dotc += 7;
                dots |= (line_mema[i] & 0x7f) << dotc;
                dotc += 7;
                i++;
            }

            // Consume pixels
            while(dotc >= 8) {
                pixeldata = (dhgr_palette[dots & 0xf] | THEN_EXTEND_3);
                dots >>= 4;
                pixeldata |= (dhgr_palette[dots & 0xf] | THEN_EXTEND_3) << 16;
                dots >>= 4;
                sl->data[sl_pos++] = pixeldata;
                dotc -= 8;
            }
        }
    }

    sl->length = sl_pos;
    sl->repeat_count = 1;
    vga_submit_scanline(sl);
}
#endif
