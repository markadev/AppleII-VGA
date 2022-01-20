#pragma once

#include <stdint.h>


extern void render_init();
extern void render_loop();

extern void update_text_flasher();
extern void render_text();
extern void render_text_line(unsigned int line);

extern void render_lores();
extern void render_mixed_lores();

extern void render_hires();
extern void render_mixed_hires();
