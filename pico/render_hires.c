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
static void render_video7_fb_hires_line(uint line);
#endif


static inline uint hires_line_to_mem_offset(uint line) {
    return ((line & 0x07) << 10) | ((line & 0x38) << 4) | (((line & 0xc0) >> 6) * 40);
}


void __time_critical_func(render_hires)(bool mixed) {
    vga_prepare_frame();
    // Skip 48 lines to center vertically
    vga_skip_lines(48);

    void (*render_hgr_line)(uint) = render_hires_line;
#ifdef APPLE_MODEL_IIE
    if(soft_80col && soft_dhires) {
        render_hgr_line = render_dhires_line;
    } else if(soft_dhires && !soft_80col && soft_80store) {
        render_hgr_line = render_video7_fb_hires_line;
    }
#endif

    for(uint line = 0; line < 160; line++) {
        render_hgr_line(line);
    }

    if(mixed) {
        for(uint line = 20; line < 24; line++) {
            render_text_line(line);
        }
    } else {
        for(uint line = 160; line < 192; line++) {
            render_hgr_line(line);
        }
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

    if(soft_scanline_emulation) {
        // Just insert a blank scanline between each rendered scanline
        sl->data[sl_pos++] = THEN_WAIT_HSYNC;
    } else {
        sl->repeat_count = 1;
    }
    sl->length = sl_pos;
    vga_submit_scanline(sl);
}


#ifdef APPLE_MODEL_IIE
static void __time_critical_func(render_dhires_line)(uint line) {
    uint sl_pos = 0;

    const int mode = soft_monochrom ? VIDEO7_MODE_560x192 : soft_video7_mode;

    const uint8_t *page = ((soft_switches & SOFTSW_PAGE_2) && !soft_80store) ? hgr_p2 : hgr_p1;
    const uint8_t *aux_page = ((soft_switches & SOFTSW_PAGE_2) && !soft_80store) ? hgr_p4 : hgr_p3;
    const uint8_t *line_mem_even = page + hires_line_to_mem_offset(line);
    const uint8_t *line_mem_odd = aux_page + hires_line_to_mem_offset(line);

    struct vga_scanline *sl = vga_prepare_scanline();

    // Pad 40 pixels on the left to center horizontally
    if(mode != VIDEO7_MODE_160x192) {
        sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels
        sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels
        sl->data[sl_pos++] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels
    }

    if(mode == VIDEO7_MODE_560x192) {
        // 560x192 monochrome mode - Ref: VIDEO-7 User's Manual section 7.6.1 and US Patent 4631692
        // Supported by the Extended 80-column text/AppleColor adapter card

        const uint32_t bits_to_pixels[4] = {
            ((uint32_t)mono_bg_color << 16) | mono_bg_color,
            ((uint32_t)mono_bg_color << 16) | mono_fg_color,
            ((uint32_t)mono_fg_color << 16) | mono_bg_color,
            ((uint32_t)mono_fg_color << 16) | mono_fg_color,
        };

        for(uint i = 0; i < 40; i++) {
            // Extract 14 bits from the next 2 display bytes
            uint32_t dots = (line_mem_odd[i] & 0x7f) | ((uint32_t)line_mem_even[i] & 0x7f) << 7;

            // Render out the pixels, least significant bit first
            for(int j = 0; j < 7; j++) {
                sl->data[sl_pos] = bits_to_pixels[dots & 0x03];
                sl_pos++;
                dots >>= 2;
            }
        }
    } else if(mode == VIDEO7_MODE_160x192) {
        // 160x192 16-color mode - Ref: VIDEO-7 User's Manual section 7.6.3 and US Patent 4631692
        for(uint i = 0; i < 40; i++) {
            // Each video memory byte contains the color of two pixels - no weird bit alignment in this mode!
            uint_fast8_t b = line_mem_odd[i];
            uint_fast16_t pix1 = lores_palette[b & 0xf] | THEN_EXTEND_3;
            uint_fast16_t pix2 = lores_palette[(b >> 4) & 0xf] | THEN_EXTEND_3;
            sl->data[sl_pos] = (uint32_t)pix1 | ((uint32_t)pix2 << 16);
            sl_pos++;

            b = line_mem_even[i];
            pix1 = lores_palette[b & 0xf] | THEN_EXTEND_3;
            pix2 = lores_palette[(b >> 4) & 0xf] | THEN_EXTEND_3;
            sl->data[sl_pos] = (uint32_t)pix1 | ((uint32_t)pix2 << 16);
            sl_pos++;
        }
    } else if(mode == VIDEO7_MODE_MIX) {
        // 160x192 mixed color/mono mode - Ref: VIDEO-7 User's Manual section 7.6.4 and US Patent 4631692
        // Supported by the Extended 80-column text/AppleColor adapter card
        for(uint i = 0; i < 40; i += 2) {
            // Load in 28 dots from the next 4 video data bytes
            uint_fast8_t vid0 = line_mem_odd[i];
            uint_fast8_t vid1 = line_mem_even[i];
            uint_fast8_t vid2 = line_mem_odd[i+1];
            uint_fast8_t vid3 = line_mem_even[i+1];

            uint32_t dots = (vid0 & 0x7f);
            uint32_t pixelmode = ((vid0 & 0x80) ? 0x7f : 0);
            dots |= (uint32_t)(vid1 & 0x7f) << 7;
            pixelmode |= ((vid1 & 0x80) ? (0x7f << 7) : 0);
            dots |= (uint32_t)(vid2 & 0x7f) << 14;
            pixelmode |= ((vid2 & 0x80) ? (0x7f << 14) : 0);
            dots |= (uint32_t)(vid3 & 0x7f) << 21;
            pixelmode |= ((vid3 & 0x80) ? (0x7f << 21) : 0);

            // Consume pixels
            uint_fast8_t dotc = 28;
            while(dotc >= 4) {
                if(pixelmode) {
                    uint32_t pixeldata = (dhgr_palette[dots & 0xf] | THEN_EXTEND_1);
                    pixeldata |= pixeldata << 16;
                    dots >>= 4;
                    pixelmode >>= 4;
                    sl->data[sl_pos++] = pixeldata;
                    dotc -= 4;
                } else {
                    uint32_t pixeldata = (dots & 1) ? dhgr_palette[15] : dhgr_palette[0];
                    dots >>= 1;
                    pixelmode >>= 1;
                    pixeldata |= (uint32_t)((dots & 1) ? dhgr_palette[15] : dhgr_palette[0]) << 16;
                    dots >>= 1;
                    pixelmode >>= 1;
                    sl->data[sl_pos++] = pixeldata;
                    dotc -= 2;
                    pixeldata = (dots & 1) ? dhgr_palette[15] : dhgr_palette[0];
                    dots >>= 1;
                    pixelmode >>= 1;
                    pixeldata |= (uint32_t)((dots & 1) ? dhgr_palette[15] : dhgr_palette[0]) << 16;
                    dots >>= 1;
                    pixelmode >>= 1;
                    sl->data[sl_pos++] = pixeldata;
                    dotc -= 2;
                }
            }
        }
    } else {
        // Standard 140x192 16-color double-hires mode
        uint32_t dots = 0;
        uint_fast8_t dotc = 0;

        for(uint i = 0; i < 40; ) {
            // Load in as many subpixels as possible
            while((dotc <= 18) && (i < 40)) {
                dots |= (line_mem_odd[i] & 0x7f) << dotc;
                dotc += 7;
                dots |= (line_mem_even[i] & 0x7f) << dotc;
                dotc += 7;
                i++;
            }

            // Consume pixels
            while(dotc >= 8) {
                uint32_t pixeldata = (dhgr_palette[dots & 0xf] | THEN_EXTEND_3);
                dots >>= 4;
                pixeldata |= (dhgr_palette[dots & 0xf] | THEN_EXTEND_3) << 16;
                dots >>= 4;
                sl->data[sl_pos++] = pixeldata;
                dotc -= 8;
            }
        }
    }

    if(soft_scanline_emulation) {
        // Just insert a blank scanline between each rendered scanline
        sl->data[sl_pos++] = THEN_WAIT_HSYNC;
    } else {
        sl->repeat_count = 1;
    }
    sl->length = sl_pos;
    vga_submit_scanline(sl);
}


// Render one line of foreground/background hires mode - Ref: VIDEO-7 User's Manual section 7.5
// Supported (but not documented?) by the Extended 80-column text/AppleColor adapter card.
// Test using the Extended 80-column text/AppleColor demo disk.
static void render_video7_fb_hires_line(uint line) {
    uint32_t pixeldata;

    const uint8_t *page = ((soft_switches & SOFTSW_PAGE_2) && !soft_80store) ? hgr_p2 : hgr_p1;
    const uint8_t *aux_page = ((soft_switches & SOFTSW_PAGE_2) && !soft_80store) ? hgr_p4 : hgr_p3;
    const uint8_t *mem_main = page + hires_line_to_mem_offset(line);
    const uint8_t *mem_aux = aux_page + hires_line_to_mem_offset(line);

    struct vga_scanline *sl = vga_prepare_scanline();
    uint sl_pos = 0;

    // Pad 40 pixels on the left to center horizontally
    sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels
    sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels
    sl->data[sl_pos++] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels

    for(uint i = 0; i < 40;) {
        uint32_t dots = (mem_main[i] & 0x7f);
        uint32_t color1 = lores_palette[(mem_aux[i] >> 4) & 0xf];
        uint32_t color2 = lores_palette[(mem_aux[i] >> 0) & 0xf];
        i++;

        dots |= (mem_main[i] & 0x7f) << 7;
        uint32_t color3 = lores_palette[(mem_aux[i] >> 4) & 0xf];
        uint32_t color4 = lores_palette[(mem_aux[i] >> 0) & 0xf];
        i++;

        for(int j = 0; j < 3; j++) {
            pixeldata = ((dots & 1) ? color1 : color2) | THEN_EXTEND_1;
            dots >>= 1;
            pixeldata |= (((dots & 1) ? color1 : color2) | THEN_EXTEND_1) << 16;
            dots >>= 1;
            sl->data[sl_pos++] = pixeldata;
        }

        pixeldata = ((dots & 1) ? color1 : color2) | THEN_EXTEND_1;
        dots >>= 1;
        pixeldata |= (((dots & 1) ? color3 : color4) | THEN_EXTEND_1) << 16;
        dots >>= 1;
        sl->data[sl_pos++] = pixeldata;

        for(int j = 0; j < 3; j++) {
            pixeldata = ((dots & 1) ? color3 : color4) | THEN_EXTEND_1;
            dots >>= 1;
            pixeldata |= (((dots & 1) ? color3 : color4) | THEN_EXTEND_1) << 16;
            dots >>= 1;
            sl->data[sl_pos++] = pixeldata;
        }
    }

    if(soft_scanline_emulation) {
        // Just insert a blank scanline between each rendered scanline
        sl->data[sl_pos++] = THEN_WAIT_HSYNC;
    } else {
        sl->repeat_count = 1;
    }
    sl->length = sl_pos;
    vga_submit_scanline(sl);
}
#endif
