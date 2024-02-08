#include "render.h"
#include "buffers.h"


void render_init() {
    generate_hires_tables();
}


void render_loop() {
    while(1) {
#ifdef RENDER_TEST_PATTERN
        render_vga_testpattern();
#else

#ifdef APPLE_MODEL_IIPLUS
        if(card_videx_80col) {
            update_videx_text_flasher();
            render_videx_text();
        } else {
#endif
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
#ifdef APPLE_MODEL_IIPLUS
        }
#endif
#endif
    }
}
