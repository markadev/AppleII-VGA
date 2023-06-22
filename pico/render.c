#include <string.h>
#include <pico/stdlib.h>
#include <hardware/timer.h>
#include "buffers.h"
#include "render.h"
#include "textfont.h"
#include "vga.h"

#define _PIXPAIR(p1, p2) ((uint32_t)(p1) | (((uint32_t)p2) << 16))

#ifdef RENDER_TEST_PATTERN
static void __noinline __time_critical_func(render_testpattern)() {
    vga_prepare_frame();

    for(uint line = 0; line < VGA_HEIGHT;) {
        struct vga_scanline* sl = vga_prepare_scanline();
        uint sl_pos = 0;

        if((line == 0) || (line == VGA_HEIGHT - 1)) {
            for(; sl_pos < VGA_WIDTH / 8; sl_pos++) {
                sl->data[sl_pos] = _PIXPAIR(0x1ff | THEN_EXTEND_3, 0x1ff | THEN_EXTEND_3);
            }
            sl->length = sl_pos;
        } else if((line == 1) || (line == VGA_HEIGHT - 32 - 1)) {
            for(uint i = 0; i < VGA_WIDTH / 4; i++, sl_pos++) {
                sl->data[sl_pos] = _PIXPAIR(0x1ff, 0);
            }
            for(uint i = 0; i < VGA_WIDTH / 4; i++, sl_pos++) {
                sl->data[sl_pos] = _PIXPAIR(0, 0x1ff);
            }

            sl->length = sl_pos;
            sl->repeat_count = 31;
        } else if((line >= 44) && (line < 44 + 128)) {
            for(uint i = 0; i < 15; i++) {
                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_3, 0 | THEN_EXTEND_3);
            }
            sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_2, 0 | THEN_EXTEND_2);
            const uint g = (line - 44) / 16;
            for(uint b = 0; b < 3; b++) {
                for(uint r = 0; r < 8; r++) {
                    const uint rgb = (r << 6) | (g << 3) | b;
                    sl->data[sl_pos++] = _PIXPAIR(rgb | THEN_EXTEND_3, rgb | THEN_EXTEND_3);
                    sl->data[sl_pos++] = _PIXPAIR(rgb | THEN_EXTEND_3, rgb | THEN_EXTEND_3);
                }
                sl->data[sl_pos++] = _PIXPAIR(0, 0);
            }
            sl->length = sl_pos;
            sl->repeat_count = 15;
        } else if((line >= 174) && (line < 174 + 128)) {
            for(uint i = 0; i < 15; i++) {
                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_3, 0 | THEN_EXTEND_3);
            }
            sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_2, 0 | THEN_EXTEND_2);

            const uint g = (line - 174) / 16;

            // center-left square
            uint b = 3;
            for(uint r = 0; r < 8; r++) {
                const uint rgb = (r << 6) | (g << 3) | b;
                sl->data[sl_pos++] = _PIXPAIR(rgb | THEN_EXTEND_3, rgb | THEN_EXTEND_3);
                sl->data[sl_pos++] = _PIXPAIR(rgb | THEN_EXTEND_3, rgb | THEN_EXTEND_3);
            }
            sl->data[sl_pos++] = _PIXPAIR(0, 0);

            // center square (black)
            for(uint i = 0; i < 8; i++) {
                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_3, 0 | THEN_EXTEND_3);
                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_3, 0 | THEN_EXTEND_3);
            }
            sl->data[sl_pos++] = _PIXPAIR(0, 0);

            // center-right square
            b = 4;
            for(uint r = 0; r < 8; r++) {
                const uint rgb = (r << 6) | (g << 3) | b;
                sl->data[sl_pos++] = _PIXPAIR(rgb | THEN_EXTEND_3, rgb | THEN_EXTEND_3);
                sl->data[sl_pos++] = _PIXPAIR(rgb | THEN_EXTEND_3, rgb | THEN_EXTEND_3);
            }
            sl->data[sl_pos++] = _PIXPAIR(0, 0);

            sl->length = sl_pos;
            sl->repeat_count = 15;
        } else if((line >= 304) && (line < 304 + 128)) {
            for(uint i = 0; i < 15; i++) {
                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_3, 0 | THEN_EXTEND_3);
            }
            sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_2, 0 | THEN_EXTEND_2);

            const uint g = (line - 304) / 16;
            for(uint b = 5; b < 8; b++) {
                for(uint r = 0; r < 8; r++) {
                    const uint rgb = (r << 6) | (g << 3) | b;
                    sl->data[sl_pos++] = _PIXPAIR(rgb | THEN_EXTEND_3, rgb | THEN_EXTEND_3);
                    sl->data[sl_pos++] = _PIXPAIR(rgb | THEN_EXTEND_3, rgb | THEN_EXTEND_3);
                }
                sl->data[sl_pos++] = _PIXPAIR(0, 0);
            }
            sl->length = sl_pos;
            sl->repeat_count = 15;
        } else {
            // black
            sl->data[sl_pos] = _PIXPAIR(0 | THEN_EXTEND_3, 0 | THEN_EXTEND_3);
            sl_pos++;
        }

        line += sl->repeat_count + 1;
        vga_submit_scanline(sl);
    }
}
#endif

void render_init() {
    // Initialize the character generator ROM
    memcpy(character_rom, default_character_rom, sizeof(character_rom));
#ifdef APPLE_MODEL_IIE
    memcpy(altcharacter_rom, alternate_character_rom, sizeof(altcharacter_rom));
#endif
}

void render_loop() {
    while(1) {
        update_text_flasher();

        switch(soft_switches & SOFTSW_MODE_MASK) {
        case 0: {
#ifdef RENDER_TEST_PATTERN
            gpio_put(PICO_DEFAULT_LED_PIN, 1);
            render_testpattern();
#else
            render_lores();
#endif
            break;
        }
        case SOFTSW_MIX_MODE: {
            render_mixed_lores();
            break;
        }
        case SOFTSW_HIRES_MODE: {
            render_hires();
            break;
        }
        case SOFTSW_HIRES_MODE | SOFTSW_MIX_MODE: {
            render_mixed_hires();
            break;
        }
        default: {
            gpio_put(PICO_DEFAULT_LED_PIN, 0);
            render_text();
            break;
        }
        }
    }
}