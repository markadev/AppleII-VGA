#include "render.h"

#include <string.h>
#include "buffers.h"
#include "textfont.h"


void render_init() {
    // Initialize the character generator ROM
    memcpy(character_rom, default_character_rom, sizeof(character_rom));

    generate_hires_tables();
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
            render_hires(false);
            break;
        case SOFTSW_HIRES_MODE | SOFTSW_MIX_MODE:
            render_hires(true);
            break;
        default:
            render_text();
            break;
        }
#endif
    }
}
