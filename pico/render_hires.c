#include "render.h"

#include <pico/stdlib.h>
#include "buffers.h"
#include "colors.h"
#include "hires_rgb_color_patterns.h"
#include "hires_dot_patterns.h"
#include "vga.h"


static void render_hires_line(uint line);
#ifdef APPLE_MODEL_IIE
static void render_dhires_line(uint line);
static void render_video7_fb_hires_line(uint line);

static uint32_t video7_mixed_lookup1[64];
static uint32_t video7_mixed_lookup2[64];
#endif


static inline uint hires_line_to_mem_offset(uint line) {
    return ((line & 0x07) << 10) | ((line & 0x38) << 4) | (((line & 0xc0) >> 6) * 40);
}


void generate_hires_tables() {
#ifdef APPLE_MODEL_IIE
    // Construct the video7_mixed_lookup1 and video7_mixed_lookup2 tables
    for(uint idx = 0; idx < 64; idx++) {
        uint32_t color1, color2;

        // The low 4 bits of the array index are the latched color that is the same for one dhires
        // pixel time (4 monochrome dots). The high 2 bits are the 'mode' bits for the 2 dots being
        // rendered (0 = monochrome, 1 = color)
        const uint32_t color = ntsc90_palette[idx & 0xf];

        // video7_mixed_lookup1 contains the pre-computed VGA pixel data for the screen dots
        // corresponding to idx[1:0] with the color mode depending on the mode bits in idx[5:4]
        color1 = (idx & 0x10) ? color : (idx & 0x01) ? ntsc90_palette[15] : ntsc90_palette[0];
        color2 = (idx & 0x20) ? color : (idx & 0x02) ? ntsc90_palette[15] : ntsc90_palette[0];
        video7_mixed_lookup1[idx] = color1 | (color2 << 16);

        // video7_mixed_lookup2 contains the pre-computed VGA pixel data for the screen dots
        // corresponding to idx[3:2] with the color mode depending on the mode bits in idx[5:4]
        color1 = (idx & 0x10) ? color : (idx & 0x04) ? ntsc90_palette[15] : ntsc90_palette[0];
        color2 = (idx & 0x20) ? color : (idx & 0x08) ? ntsc90_palette[15] : ntsc90_palette[0];
        video7_mixed_lookup2[idx] = color1 | (color2 << 16);
    }
#endif
}


void render_hires(bool mixed) {
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
            render_text_line(line, false);
        }
    } else {
        for(uint line = 160; line < 192; line++) {
            render_hgr_line(line);
        }
    }
}


static void render_hires_line(uint line) {
    struct vga_scanline *sl = vga_prepare_scanline();
    uint sl_pos = 0;

    const uint8_t *page = is_page2_display_enabled() ? hires_mainmem_page2 : hires_mainmem_page1;
    const uint8_t *line_mem = page + hires_line_to_mem_offset(line);

    // Pad 40 pixels on the left to center horizontally
    sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels
    sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels
    sl->data[sl_pos++] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels

    // Each hires byte contains 7 pixels which may be shifted right 1/2 a pixel.
    // That is represented here by 14 'dots' to precisely describe the half-pixel
    // positioning.
    //
    // For colored pixels, inspect a window of 7 dots around the pixel to determine the
    // precise dot locations and colors from a pre-computed lookup table.
    //
    // Dots would be scanned out to the CRT from LSB to MSB (right to left here):
    //
    //                          next                  |         |   previous
    //                          dots                  |         |     dots
    //        +----------------------------------------------------------------+
    // dots:   ... | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
    //                                 |              |         |              |
    //                                 \______________|_________|______________/
    //                                                |         |
    //                                                \_________/
    //                                                  current
    //                                                   pixel

    if(soft_monochrom) {
        // translate bits to rendered pixels (LSB first)
        const uint32_t bits_to_pixels[4] = {
            ((uint32_t)mono_bg_color << 16) | mono_bg_color,
            ((uint32_t)mono_bg_color << 16) | mono_fg_color,
            ((uint32_t)mono_fg_color << 16) | mono_bg_color,
            ((uint32_t)mono_fg_color << 16) | mono_fg_color,
        };

        // Load the first 14 dots
        uint32_t dots = (uint32_t)hires_dot_patterns[line_mem[0]];

        for(uint i = 1; i < 41; i++) {
            if(i < 40) {
                // Load in 14 more dots from the next video data byte
                uint b = line_mem[i];
                if(b & 0x80) {
                    // Extend the last bit from the previous byte
                    dots |= (dots & (1u << 13)) << 1;
                }
                dots |= (uint32_t)hires_dot_patterns[b] << 14;
            }

            // Render the next 14 dots
            for(uint j = 0; j < 7; j++) {
                sl->data[sl_pos] = bits_to_pixels[dots & 0x3];
                sl_pos++;
                dots >>= 2;
            }
        }
    } else {
        uint color_phase = 0;

        // Load the first 14 dots
        uint32_t dots = (uint32_t)hires_dot_patterns[line_mem[0]] << 3;

        for(uint i = 1; i < 41; i++) {
            if(i < 40) {
                // Load in 14 more dots from the next video data byte
                uint b = line_mem[i];
                if(b & 0x80) {
                    // Extend the last bit from the previous byte (bit 16 into bit 17)
                    dots |= (dots & (1u << 16)) << 1;
                }
                dots |= (uint32_t)hires_dot_patterns[b] << 17;
            }

            // Render the next 14 dots
            for(uint j = 0; j < 7; j++) {
                sl->data[sl_pos] = hires_rgb_color_patterns[color_phase | (dots & 0xff)];
                sl_pos++;
                dots >>= 2;
                color_phase ^= 0x100;
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


#ifdef APPLE_MODEL_IIE
static void render_dhires_line(uint line) {
    uint sl_pos = 0;

    // Don't support soft-monochrome mode for the 160x192 RGB mode. It could be done but it would
    // be a nonstandard extension of the Video-7 mode, which itself is rarely used.
    const int mode = (soft_monochrom && (soft_video7_mode != VIDEO7_MODE_160x192)) ? VIDEO7_MODE_560x192 : soft_video7_mode;

    const uint8_t *page = is_page2_display_enabled() ? hires_mainmem_page2 : hires_mainmem_page1;
    const uint8_t *aux_page = is_page2_display_enabled() ? hires_auxmem_page2 : hires_auxmem_page1;
    const uint8_t *line_main = page + hires_line_to_mem_offset(line);
    const uint8_t *line_aux = aux_page + hires_line_to_mem_offset(line);

    struct vga_scanline *sl = vga_prepare_scanline();

    // Pad 40 pixels on the left to center horizontally
    if(mode != VIDEO7_MODE_160x192) {
        sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels
        sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels
        sl->data[sl_pos++] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels
    }

    if(mode == VIDEO7_MODE_560x192) {
        // 560x192 monochrome mode - Ref: Video-7 RGB-SL7 User's Manual section 7.6.1 and US Patent 4631692
        // Supported by the Extended 80-column text/AppleColor adapter card

        // In soft monochrome mode use the configured "monochrome" color, otherwise use the color palette
        const uint32_t bg_color = soft_monochrom ? mono_bg_color : ntsc_palette[0];
        const uint32_t fg_color = soft_monochrom ? mono_fg_color : ntsc_palette[15];
        const uint32_t bits_to_pixels[4] = {
            (bg_color << 16) | bg_color,
            (bg_color << 16) | fg_color,
            (fg_color << 16) | bg_color,
            (fg_color << 16) | fg_color,
        };

        for(uint i = 0; i < 40; i++) {
            // Extract 14 bits from the next 2 display bytes
            uint32_t dots = (line_aux[i] & 0x7f) | ((uint32_t)line_main[i] & 0x7f) << 7;

            // Render out the pixels, least significant bit first
            for(int j = 0; j < 7; j++) {
                sl->data[sl_pos] = bits_to_pixels[dots & 0x03];
                sl_pos++;
                dots >>= 2;
            }
        }
    } else if(mode == VIDEO7_MODE_160x192) {
        // 160x192 16-color mode - Ref: Video-7 RGB-SL7 User's Manual section 7.6.3 and US Patent 4631692
        // Supported by the Extended 80-column text/AppleColor adapter card but not documented in the manual
        for(uint i = 0; i < 40; i++) {
            // Each video memory byte contains the color of two pixels - no weird bit alignment in this mode!
            uint_fast8_t b = line_aux[i];
            uint_fast16_t pix1 = ntsc_palette[b & 0xf] | THEN_EXTEND_3;
            uint_fast16_t pix2 = ntsc_palette[(b >> 4) & 0xf] | THEN_EXTEND_3;
            sl->data[sl_pos] = (uint32_t)pix1 | ((uint32_t)pix2 << 16);
            sl_pos++;

            b = line_main[i];
            pix1 = ntsc_palette[b & 0xf] | THEN_EXTEND_3;
            pix2 = ntsc_palette[(b >> 4) & 0xf] | THEN_EXTEND_3;
            sl->data[sl_pos] = (uint32_t)pix1 | ((uint32_t)pix2 << 16);
            sl_pos++;
        }
    } else if(mode == VIDEO7_MODE_MIX) {
        // 160x192 mixed color/mono mode - Ref: Video-7 RGB-SL7 User's Manual section 7.6.4 and US Patent 4631692
        // Supported by the Extended 80-column text/AppleColor adapter card

        for(uint i = 0; i < 40; i += 2) {
            // Load in 28 dots from the next 4 video data bytes. Also load in the 'mode' for each 7 bit
            // sequence that controls whether each 7 bit sequence is rendered as mono or color.
            uint_fast8_t vdata0 = line_aux[i];
            uint_fast8_t vdata1 = line_main[i];
            uint_fast8_t vdata2 = line_aux[i + 1];
            uint_fast8_t vdata3 = line_main[i + 1];

            uint32_t dots = (vdata3 & 0x7f);
            dots = (dots << 7) | (vdata2 & 0x7f);
            dots = (dots << 7) | (vdata1 & 0x7f);
            dots = (dots << 7) | (vdata0 & 0x7f);

            uint32_t pixelmode = (vdata3 & 0x80);
            pixelmode = (pixelmode << 7) | (vdata2 & 0x80);
            pixelmode = (pixelmode << 7) | (vdata1 & 0x80);
            pixelmode = (pixelmode << 7) | (vdata0 & 0x80);
            // Bits 7, 14, 21 & 28 contain the mode bits from the video data bytes
            // Extend them to fill bit ranges [10:4], [17:11], [24:18] & [31:25] respectively.
            pixelmode |= (pixelmode << 1);
            pixelmode |= (pixelmode << 2);
            pixelmode |= (pixelmode >> 3);

            // In this "mixed" mode, bit 7 of each video memory byte controls whether the bits in the byte should be rendered as
            // color or monochrome.
            //
            // This is really weird when you get into the corner case of having an unaligned 'mode' bit change that causes what
            // would normally be a sequence of 4 colored dots (a.k.a. a dhires pixel) to be split into a fractional dhires pixel
            // and some monochrome dots.
            //
            // Since the 'mode' can be changed every 7 bits and a color is represented with 4 bits then we know that the mode bit &
            // color bit boundaries coincide every 28 bits (4 video memory bytes). So we only really need to worry about these
            // fractional dhires pixels within each quad of video data bytes.
            //
            // Given the 28 bits taken from the 7 LSBs of a quad of video data bytes (bits are shifted out from LSB to MSB):
            //
            //   +---------------------------------------------------------------------------------------------------------------+
            //   | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 |   |   |   |   |   |   |   |   |   |   |
            //   | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
            //   +---------------------------------------------------------------------------------------------------------------+
            //   |               |           |   |               |       |       |               |   |           |               |
            //   \_______________/\__________|___/\______________/\______|_______/\______________/\__|___________/\______________/
            //   |   pixel 'g'       pixel 'f'       pixel 'e'       pixel 'd'        pixel 'c'      |pixel 'b'       pixel 'a'  |
            //   |                           |                           |                           |                           |
            //   |                           |                           |                           |                           |
            //   \___________________________/\__________________________/\__________________________/\__________________________/
            //         video data byte 3            video data byte 2          video data byte 1           video data byte 0
            //
            //  * pixels 'a', 'c', 'e', and 'g' will always be either one full dhires colored pixel or 4 monochrome dots,
            //    based on the mode of the byte that it came from
            //  * pixel 'b' can be split into either 3 mono dots + 1/4 of a colored dhires pixel, or 3/4 of a colored dhires pixel + 1
            //    monochrome dot, depending on the direction that the mode bit changes
            //  * pixel 'd' can be split similarly as pixel 'b' but in half instead of quartered, paired with two monochrome dots
            //  * pixel 'f' can be split similarly as pixel 'b'
            //
            // Additionally, the color chosen for the fractional dhires pixels is always going to be the discrete color defined by the
            // bits in 'b', 'd', or 'f'. It's not like hires artifact coloring where the dot positions define the colors.

            for(uint j = 0; j < 7; j++) {
                sl->data[sl_pos] = video7_mixed_lookup1[(pixelmode & 0x30) | (dots & 0xf)];
                sl_pos++;
                pixelmode >>= 2;

                sl->data[sl_pos] = video7_mixed_lookup2[(pixelmode & 0x30) | (dots & 0xf)];
                sl_pos++;
                pixelmode >>= 2;

                dots >>= 4;
            }
        }
    } else {
        // Standard 140x192 16-color double-hires mode
        uint32_t dots = 0;
        uint_fast8_t dotc = 0;

        for(uint i = 0; i < 40; i++) {
            // Load in 14 more bits from the next 2 video data bytes
            dots |= (line_aux[i] & 0x7f) << dotc;
            dotc += 7;
            dots |= (line_main[i] & 0x7f) << dotc;
            dotc += 7;

            // Convert each 4-bit sequence into the dhires colored pixel
            while(dotc >= 8) {
                uint32_t pixeldata = (ntsc90_palette[dots & 0xf] | THEN_EXTEND_3);
                dots >>= 4;
                pixeldata |= (ntsc90_palette[dots & 0xf] | THEN_EXTEND_3) << 16;
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


// Render one line of foreground/background hires mode - Ref: Video-7 RGB-SL7 User's Manual section 7.5
// Supported (but not documented?) by the Extended 80-column text/AppleColor adapter card.
// Test using the Extended 80-column text/AppleColor demo disk.
static void render_video7_fb_hires_line(uint line) {
    uint32_t pixeldata;

    const uint8_t *page = is_page2_display_enabled() ? hires_mainmem_page2 : hires_mainmem_page1;
    const uint8_t *aux_page = is_page2_display_enabled() ? hires_auxmem_page2 : hires_auxmem_page1;
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
        uint32_t color1 = ntsc_palette[(mem_aux[i] >> 4) & 0xf];
        uint32_t color2 = ntsc_palette[(mem_aux[i] >> 0) & 0xf];
        i++;

        dots |= (mem_main[i] & 0x7f) << 7;
        uint32_t color3 = ntsc_palette[(mem_aux[i] >> 4) & 0xf];
        uint32_t color4 = ntsc_palette[(mem_aux[i] >> 0) & 0xf];
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
