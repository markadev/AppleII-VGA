#pragma once

#include <stdint.h>


enum {
    SOFTSW_TEXT_MODE    = 0x01,
    SOFTSW_MIX_MODE     = 0x02,
    SOFTSW_HIRES_MODE   = 0x04,
    SOFTSW_MODE_MASK    = 0x07,

    SOFTSW_PAGE_2       = 0x08,
};

extern volatile uint32_t soft_switches;
extern uint8_t text_memory[2*1024];
extern uint8_t hires_memory[2*8192];
