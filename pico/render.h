#pragma once

// Uncomment to enable test patter generator
// #define RENDER_TEST_PATTERN

// Experimental!
// #define ENABLE_SCANLINE_EFFECT 1

extern void render_init();
extern void render_loop();

extern void render_vga_testpattern();

extern void update_text_flasher();
extern void render_text();
extern void render_text_line(unsigned int line);

extern void render_lores();
extern void render_mixed_lores();

extern void render_hires();
extern void render_mixed_hires();
