#pragma once

#include <stdint.h>

enum
{
  SOFTSW_TEXT_MODE = 0x01,
  SOFTSW_MIX_MODE = 0x02,
  SOFTSW_HIRES_MODE = 0x04,
  SOFTSW_MODE_MASK = 0x07,

  SOFTSW_PAGE_2 = 0x08,
};

enum
{
  SOFTSW_ALTCHAR_OFF = 0x00,
  SOFTSW_ALTCHAR_ON = 0x01,
};

enum
{
  SOFTSW_80COL_OFF = 0x00,
  SOFTSW_80COL_ON = 0x01,
};
enum
{
  SOFTSW_AN3_OFF = 0x00,
  SOFTSW_AN3_ON = 0x01,
};

enum
{
  SOFTSW_IOUDIS_OFF = 0x00,
  SOFTSW_IOUDIS_ON = 0x01,
};
enum
{
  SOFTSW_80STORE_OFF = 0x00,
  SOFTSW_80STORE_ON = 0x01
};
enum
{
  SOFTSW_WRITE_MAIN = 0x00,
  SOFTSW_WRITE_AUX = 0x01
};
enum
{
  SOFTSW_READ_MAIN = 0x00,
  SOFTSW_READ_AUX = 0x01
};
enum
{
  SOFTSW_DHIRES_OFF = 0x00,
  SOFTSW_DHIRES_ON = 0x01
};

extern volatile uint32_t soft_switches;
extern volatile uint32_t soft_switches_alt;
extern volatile uint32_t soft_80col;
extern volatile uint32_t soft_ioudis;
extern volatile uint32_t soft_an3;
extern volatile uint32_t soft_ramwrt;
extern volatile uint32_t soft_ramrd;
extern volatile uint32_t soft_80store;
extern volatile uint32_t soft_dhires;
extern volatile uint32_t soft_monochrome;

extern uint8_t character_rom[256 * 8];
extern uint8_t altcharacter_rom[256 * 8];
extern uint8_t text_memory[2 * 1024];
extern uint8_t aux_memory[2*1024];
extern uint8_t hires_memory[2 * 8192];
extern uint8_t dhires_aux_memory[2*8192];