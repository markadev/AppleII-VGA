#pragma once

#include <stdint.h>

// Uncomment to enable test patter generator
// #define RENDER_TEST_PATTERN

#define _RGB333(r, g, b)                                                                               \
  ((((((uint16_t)(r)*256 / 36) + 128) / 256) << 6) | (((((uint16_t)(g)*256 / 36) + 128) / 256) << 3) | \
   ((((uint16_t)(b)*256 / 36) + 128) / 256))
static uint16_t lores_palette[16] = {
    _RGB333(0x00, 0x00, 0x00), // black
    _RGB333(0xdd, 0x00, 0x33), // magenta (deep red)
    _RGB333(0x00, 0x00, 0x99), // d.blue
    _RGB333(0xdd, 0x22, 0xdd), // h.violet
    _RGB333(0x00, 0x77, 0x22), // d.green
    _RGB333(0x55, 0x55, 0x55), // d.gray
    _RGB333(0x22, 0x22, 0xff), // h.blue
    _RGB333(0x66, 0xaa, 0xff), // l.blue
    _RGB333(0x88, 0x55, 0x00), // brown
    _RGB333(0xff, 0x66, 0x00), // h.orange
    _RGB333(0xaa, 0xaa, 0xaa), // l.gray
    _RGB333(0xff, 0x99, 0x88), // pink
    _RGB333(0x11, 0xdd, 0x00), // h.green
    _RGB333(0xff, 0xff, 0x00), // yellow
    _RGB333(0x44, 0xff, 0x99), // aqua
    _RGB333(0xff, 0xff, 0xff), // white
};

static uint16_t dhgr_palette[16] = {
    _RGB333(0x00, 0x00, 0x00),  // black - 0
    _RGB333(0x00, 0x00, 0x99),  // d.blue
    _RGB333(0x00, 0x77, 0x22),  // d.green
    _RGB333(0x22, 0x22, 0xff),  // h.blue
    _RGB333(0x88, 0x55, 0x00),  // brown
    _RGB333(0xaa, 0xaa, 0xaa),  // l.gray
    _RGB333(0x11, 0xdd, 0x00),  // h.green
    _RGB333(0x44, 0xff, 0x99),  // aqua
    _RGB333(0xdd, 0x00, 0x33),  // magenta (deep red)
    _RGB333(0xdd, 0x22, 0xdd),  // h.violet
    _RGB333(0x55, 0x55, 0x55),  // d.gray
    _RGB333(0x66, 0xaa, 0xff),  // l.blue
    _RGB333(0xff, 0x66, 0x00),  // h.orange
    _RGB333(0xff, 0x99, 0x88),  // pink
    _RGB333(0xff, 0xff, 0x00),  // yellow
    _RGB333(0xff, 0xff, 0xff),  // white 
};
#undef _RGB333

extern void render_init();
extern void render_loop();

extern void update_text_flasher();
extern void render_text();
extern void render_text_line(unsigned int line);
extern void render_text80_line(unsigned int line);

extern void render_lores();
extern void render_mixed_lores();

extern void render_hires();
extern void render_mixed_hires();
