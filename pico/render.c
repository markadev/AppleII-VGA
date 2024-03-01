#include "render.h"
#include "buffers.h"
#ifdef APPLE_MODEL_IIPLUS
#include "videx_vterm.h"
#endif


void render_init() {
    generate_hires_tables();
}


void render_loop() {
#ifdef RENDER_TEST_PATTERN
    while(1) {
        render_vga_testpattern();
    }
#else
    while(1) {
        update_text_flasher();
#ifdef APPLE_MODEL_IIPLUS
        update_videx_text_flasher();
#endif

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
#ifdef APPLE_MODEL_IIPLUS
            if(videx_vterm_80col_enabled) {
                render_videx_text();
            } else
#endif
            {
                render_text();
            }
            break;
        }
    }
#endif
}
