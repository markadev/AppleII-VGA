#pragma once

#include <stdint.h>
#include <stdbool.h>


enum {
    SOFTSW_TEXT_MODE = 0x01,
    SOFTSW_MIX_MODE = 0x02,
    SOFTSW_HIRES_MODE = 0x04,
    SOFTSW_MODE_MASK = 0x07,
    SOFTSW_PAGE_2 = 0x08,
};

enum {
    SOFTSW_ALTCHAR_OFF = 0x00,
    SOFTSW_ALTCHAR_ON = 0x01,
};

enum {
    SOFTSW_80COL_OFF = 0x00,
    SOFTSW_80COL_ON = 0x01,
};

enum {
    SOFTSW_IOUDIS_OFF = 0x00,
    SOFTSW_IOUDIS_ON = 0x01,
};

enum { SOFTSW_DHIRES_OFF = 0x00, SOFTSW_DHIRES_ON = 0x01 };

enum { SOFTSW_MONO_DIS = 0, SOFTSW_MONO_EN = 1 };

enum {
    VIDEO7_MODE_140x192 = 0,
    VIDEO7_MODE_160x192 = 1,
    VIDEO7_MODE_MIX = 2,
    VIDEO7_MODE_560x192 = 3,
};

extern volatile uint32_t soft_switches;
extern volatile uint32_t soft_switches_alt;
extern volatile uint32_t soft_80col;
extern volatile uint32_t soft_ioudis;
extern volatile uint32_t soft_dhires;
extern volatile uint32_t soft_monochrom;
extern volatile uint32_t soft_video7_mode;
extern volatile bool soft_ramwrt;
extern volatile bool soft_80store;

extern uint8_t character_rom[256 * 8];
#ifdef APPLE_MODEL_IIE
extern uint8_t altcharacter_rom[256 * 8];
#endif

extern uint8_t main_memory[48 * 1024];
extern uint8_t aux_memory[48 * 1024];
extern uint8_t *text_p1;
extern uint8_t *text_p2;
extern uint8_t *text_p3;
extern uint8_t *text_p4;
extern uint8_t *hgr_p1;
extern uint8_t *hgr_p2;
extern uint8_t *hgr_p3;
extern uint8_t *hgr_p4;
