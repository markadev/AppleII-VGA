#pragma once

#include <stdint.h>

// Uncomment to enable test patter generator
// #define RENDER_TEST_PATTERN

#define _RGB333(r, g, b)                                                                               \
  ((((((uint16_t)(r)*256 / 36) + 128) / 256) << 6) | (((((uint16_t)(g)*256 / 36) + 128) / 256) << 3) | \
   ((((uint16_t)(b)*256 / 36) + 128) / 256))
static uint16_t lores_palette[16] = {
    _RGB333(0x00, 0x00, 0x00),  // black - 0 !
    _RGB333(0xd0, 0x00, 0x30),  // magenta - 1 !
    _RGB333(0x00, 0x00, 0x80),  // d.blue - 2 !
    _RGB333(0x80, 0x00, 0x80),  // h.violet - 3 !
    _RGB333(0x00, 0x80, 0x00),  // d.green - 4 !
    _RGB333(0x2f, 0x2f, 0x2f),  // d.gray - 5 !
    _RGB333(0x00, 0x00, 0xff),  // h.blue - 6 !
    _RGB333(0xad, 0xd8, 0xe6),  // l.blue - 7 !
    _RGB333(0x80, 0x50, 0x00),  // brown - 8 !
    _RGB333(0xff, 0x5f, 0x1f),  // h.orange - 9 !
    _RGB333(0x8f, 0x8f, 0x8f),  // l.gray - 10
    _RGB333(0xe3, 0x0b, 0x5c),  // pink - 11 !
    _RGB333(0x00, 0xff, 0x00),  // h.green - 12 !
    _RGB333(0xff, 0xff, 0x08),  // yellow - 13 !
    _RGB333(0x00, 0xff, 0xff),  // aqua - 14 !
    _RGB333(0xff, 0xff, 0xff),  // white - 15 !
};

static uint16_t dhgr_palette[16] = {
    _RGB333(0x00, 0x00, 0x00),  // black - 0
    _RGB333(0x00, 0x00, 0x80),  // d.blue - 2
    _RGB333(0x00, 0x80, 0x00),  // d.green - 4
    _RGB333(0x00, 0x00, 0xff),  // h.blue - 6
    _RGB333(0x80, 0x50, 0x00),  // brown - 8
    _RGB333(0x8f, 0x8f, 0x8f),  // l.gray - 10
    _RGB333(0x00, 0xff, 0x00),  // h.green - 12
    _RGB333(0x00, 0xff, 0xff),  // aqua - 14
    _RGB333(0xd0, 0x00, 0x30),  // magenta - 1
    _RGB333(0x80, 0x00, 0x80),  // h.violet - 3
    _RGB333(0x2f, 0x2f, 0x2f),  // d.gray - 5
    _RGB333(0xad, 0xd8, 0xe6),  // l.blue - 7
    _RGB333(0xff, 0x5f, 0x1f),  // h.orange - 9
    _RGB333(0xe3, 0x0b, 0x5c),  // pink - 11
    _RGB333(0xff, 0xff, 0x08),  // yellow - 13
    _RGB333(0xff, 0xff, 0xff),  // white - 15
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
