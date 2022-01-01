#pragma once

#include <stdint.h>


extern void render_init();
extern void render_loop();

extern void update_text_flasher();
extern void render_text(uint8_t *page);
extern void render_text_line(uint8_t *page, unsigned int line);

extern void render_lores(uint8_t *page);
extern void render_mixed_lores(uint8_t *page);

extern void render_hires(uint8_t *page);
extern void render_mixed_hires(uint8_t *hires_page, uint8_t *text_page);
