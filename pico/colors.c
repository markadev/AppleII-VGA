#include "colors.h"

// clang-format off
#define RGB333(r, g, b) (uint16_t)( \
    (((((unsigned int)(r) * 256 / 36) + 128) / 256) << 6) | \
    (((((unsigned int)(g) * 256 / 36) + 128) / 256) << 3) | \
    ((((unsigned int)(b) * 256 / 36) + 128) / 256) \
)

#define COLOR_BLACK     RGB333(0x00, 0x00, 0x00)
#define COLOR_MAGENTA   RGB333(0xdd, 0x00, 0x33)
#define COLOR_D_BLUE    RGB333(0x00, 0x00, 0x99)
#define COLOR_H_VIOLET  RGB333(0xdd, 0x22, 0xdd)
#define COLOR_D_GREEN   RGB333(0x00, 0x77, 0x22)
#define COLOR_D_GRAY    RGB333(0x55, 0x55, 0x55)
#define COLOR_H_BLUE    RGB333(0x22, 0x22, 0xff)
#define COLOR_L_BLUE    RGB333(0x66, 0xaa, 0xff)
#define COLOR_BROWN     RGB333(0x88, 0x55, 0x00)
#define COLOR_H_ORANGE  RGB333(0xff, 0x66, 0x00)
#define COLOR_L_GRAY    RGB333(0xaa, 0xaa, 0xaa)
#define COLOR_PINK      RGB333(0xff, 0x99, 0x88)
#define COLOR_H_GREEN   RGB333(0x11, 0xdd, 0x00)
#define COLOR_YELLOW    RGB333(0xff, 0xff, 0x00)
#define COLOR_AQUA      RGB333(0x44, 0xff, 0x99)
#define COLOR_WHITE     RGB333(0xff, 0xff, 0xff)
// clang-format on

const uint16_t lores_palette[16] = {
    COLOR_BLACK,
    COLOR_MAGENTA,
    COLOR_D_BLUE,
    COLOR_H_VIOLET,
    COLOR_D_GREEN,
    COLOR_D_GRAY,
    COLOR_H_BLUE,
    COLOR_L_BLUE,
    COLOR_BROWN,
    COLOR_H_ORANGE,
    COLOR_L_GRAY,
    COLOR_PINK,
    COLOR_H_GREEN,
    COLOR_YELLOW,
    COLOR_AQUA,
    COLOR_WHITE,
};

const uint16_t dhgr_palette[16] = {
    COLOR_BLACK,
    COLOR_D_BLUE,
    COLOR_D_GREEN,
    COLOR_H_BLUE,
    COLOR_BROWN,
    COLOR_L_GRAY,
    COLOR_H_GREEN,
    COLOR_AQUA,
    COLOR_MAGENTA,
    COLOR_H_VIOLET,
    COLOR_D_GRAY,
    COLOR_L_BLUE,
    COLOR_H_ORANGE,
    COLOR_PINK,
    COLOR_YELLOW,
    COLOR_WHITE,
};
