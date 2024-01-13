#include "render.h"

#include <pico/stdlib.h>
#include "vga.h"


#define _PIXPAIR(p1, p2) ((uint32_t)(p1) | (((uint32_t)p2) << 16))


// Render a test pattern for testing the VGA output directly
void render_vga_testpattern() {
    vga_prepare_frame();

    for(uint line = 0; line < VGA_HEIGHT;) {
        struct vga_scanline *sl = vga_prepare_scanline();
        uint sl_pos = 0;

        if((line == 0) || (line == VGA_HEIGHT - 1)) {
            // A white line across the top & bottom of the screen
            for(; sl_pos < VGA_WIDTH / 16; sl_pos++) {
                sl->data[sl_pos] = _PIXPAIR(0x1ff | THEN_EXTEND_7, 0x1ff | THEN_EXTEND_7);
            }
            sl->length = sl_pos;
        } else if((line == 1) || (line == VGA_HEIGHT - 32 - 1)) {
            // An alternating pattern of vertical white & black lines
            for(uint i = 0; i < VGA_WIDTH / 4; i++, sl_pos++) {
                sl->data[sl_pos] = _PIXPAIR(0x1ff, 0);
            }
            for(uint i = 0; i < VGA_WIDTH / 4; i++, sl_pos++) {
                sl->data[sl_pos] = _PIXPAIR(0, 0x1ff);
            }

            sl->length = sl_pos;
            sl->repeat_count = 31;
        } else if((line >= 44) && (line < 44 + 128)) {
            // The first row of color squares
            for(uint i = 0; i < 7; i++) {
                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_7, 0 | THEN_EXTEND_7);
            }
            sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_6, 0 | THEN_EXTEND_6);

            const uint g = (line - 44) / 16;
            for(uint b = 0; b < 3; b++) {
                for(uint r = 0; r < 8; r++) {
                    const uint rgb = (r << 6) | (g << 3) | b;
                    sl->data[sl_pos++] = _PIXPAIR(rgb | THEN_EXTEND_7, rgb | THEN_EXTEND_7);
                }
                sl->data[sl_pos++] = _PIXPAIR(0, 0);
            }

            sl->length = sl_pos;
            sl->repeat_count = 15;
        } else if((line >= 174) && (line < 174 + 128)) {
            // The second row of color squares
            for(uint i = 0; i < 7; i++) {
                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_7, 0 | THEN_EXTEND_7);
            }
            sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_6, 0 | THEN_EXTEND_6);

            const uint g = (line - 174) / 16;

            // center-left square
            uint b = 3;
            for(uint r = 0; r < 8; r++) {
                const uint rgb = (r << 6) | (g << 3) | b;
                sl->data[sl_pos++] = _PIXPAIR(rgb | THEN_EXTEND_7, rgb | THEN_EXTEND_7);
            }
            sl->data[sl_pos++] = _PIXPAIR(0, 0);

            // center square (black)
            for(uint i = 0; i < 8; i++) {
                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_7, 0 | THEN_EXTEND_7);
            }
            sl->data[sl_pos++] = _PIXPAIR(0, 0);

            // center-right square
            b = 4;
            for(uint r = 0; r < 8; r++) {
                const uint rgb = (r << 6) | (g << 3) | b;
                sl->data[sl_pos++] = _PIXPAIR(rgb | THEN_EXTEND_7, rgb | THEN_EXTEND_7);
            }
            sl->data[sl_pos++] = _PIXPAIR(0, 0);

            sl->length = sl_pos;
            sl->repeat_count = 15;
        } else if((line >= 304) && (line < 304 + 128)) {
            // The third row of color squares
            for(uint i = 0; i < 7; i++) {
                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_7, 0 | THEN_EXTEND_7);
            }
            sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_6, 0 | THEN_EXTEND_6);

            const uint g = (line - 304) / 16;
            for(uint b = 5; b < 8; b++) {
                for(uint r = 0; r < 8; r++) {
                    const uint rgb = (r << 6) | (g << 3) | b;
                    sl->data[sl_pos++] = _PIXPAIR(rgb | THEN_EXTEND_7, rgb | THEN_EXTEND_7);
                }
                sl->data[sl_pos++] = _PIXPAIR(0, 0);
            }

            sl->length = sl_pos;
            sl->repeat_count = 15;
        } else {
            // All other lines are just black
            sl->data[sl_pos] = _PIXPAIR(0, 0);
            sl_pos++;
        }

        line += sl->repeat_count + 1;
        vga_submit_scanline(sl);
    }
}
