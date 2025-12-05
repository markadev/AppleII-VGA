#include "buffers.h"

// Shadow copy of the Apple soft-switches
volatile uint32_t soft_switches;
volatile uint32_t soft_video7_mode;
volatile bool soft_80col;
volatile bool soft_80store;
volatile bool soft_altcharset;
volatile bool soft_dhires;
volatile bool soft_monochrom;
volatile bool soft_ramwrt;

// Custom device soft-switches
volatile bool soft_scanline_emulation;
volatile bool soft_force_alt_textcolor;
volatile bool soft_smooth_hires;


// The currently programmed character generator ROM for text mode
uint8_t character_rom[256 * 8];

// The lower 24K of main and aux memory (on IIe) where the video memory resides
uint8_t main_memory[24 * 1024];
uint8_t aux_memory[24 * 1024];


uint8_t *text_mainmem_page1 = main_memory + 0x0400;
uint8_t *text_mainmem_page2 = main_memory + 0x0800;
uint8_t *text_auxmem_page1 = aux_memory + 0x0400;
uint8_t *text_auxmem_page2 = aux_memory + 0x0800;
uint8_t *hires_mainmem_page1 = main_memory + 0x2000;
uint8_t *hires_mainmem_page2 = main_memory + 0x4000;
uint8_t *hires_auxmem_page1 = aux_memory + 0x2000;
uint8_t *hires_auxmem_page2 = aux_memory + 0x4000;
