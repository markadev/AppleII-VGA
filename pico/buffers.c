#include "buffers.h"

// Shadow copy of the Apple soft-switches
volatile uint32_t soft_switches;
volatile uint32_t soft_switches_alt;
volatile uint32_t soft_80col;
volatile uint32_t soft_an3;
volatile uint32_t soft_ioudis;
volatile uint32_t soft_ramwrt;
volatile uint32_t soft_ramrd;
volatile uint32_t soft_80store;
volatile uint32_t soft_dhires;

// The currently programmed character generator ROM for text mode
uint8_t character_rom[256 * 8];
uint8_t altcharacter_rom[256 * 8];

// Shadow copy of Apple memory locations $0400 - $0BFF
uint8_t text_memory[2 * 1024];
// Shadow copy of Aux memory locations $0400 - $0BFF (for 80 Columns)
uint8_t aux_memory[2*1024];

// Shadow copy of Apple memory locations $2000 - $5FFF
uint8_t hires_memory[2 * 8192];

// Shadow copy of Aux memory locations $2000 - $5FFF (for DHIRES)
uint8_t dhires_aux_memory[2*8192];
