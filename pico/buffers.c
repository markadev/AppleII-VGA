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
volatile uint32_t soft_monochrom;
volatile uint32_t soft_video7;

// The currently programmed character generator ROM for text mode
uint8_t character_rom[256 * 8];
#ifdef APPLE_MODEL_IIE
uint8_t altcharacter_rom[256 * 8];
#endif

uint8_t main_memory[48 * 1024];
uint8_t aux_memory[48 * 1024];


volatile uint8_t *text_p1 = main_memory + 0x0400;
volatile uint8_t *text_p2 = main_memory + 0x0800;
volatile uint8_t *text_p3 = aux_memory + 0x0400;
volatile uint8_t *text_p4 = aux_memory + 0x0800;
volatile uint8_t *hgr_p1 = main_memory + 0x2000;
volatile uint8_t *hgr_p2 = main_memory + 0x4000;
volatile uint8_t *hgr_p3 = aux_memory + 0x2000;
volatile uint8_t *hgr_p4 = aux_memory + 0x4000;