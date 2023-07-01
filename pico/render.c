#include "render.h"

#include <string.h>
#include <hardware/timer.h>
#include "buffers.h"
#include "textfont.h"
#include "vga.h"


void render_init() {
    // Initialize the character generator ROM
    memcpy(character_rom, default_character_rom, sizeof(character_rom));
#ifdef APPLE_MODEL_IIE
    memcpy(altcharacter_rom, alternate_character_rom, sizeof(altcharacter_rom));
#endif
}

void render_loop() {
    while(1) {
#ifdef RENDER_TEST_PATTERN
        render_vga_testpattern();
#else
        update_text_flasher();

        switch(soft_switches & SOFTSW_MODE_MASK) {
        case 0:
            render_lores();
            break;
        case SOFTSW_MIX_MODE:
            render_mixed_lores();
            break;
        case SOFTSW_HIRES_MODE:
            render_hires();
            break;
        case SOFTSW_HIRES_MODE | SOFTSW_MIX_MODE:
            render_mixed_hires();
            break;
        default:
            render_text();
            break;
        }
#endif
    }
}
