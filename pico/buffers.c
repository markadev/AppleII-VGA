#include "buffers.h"


// Shadow copy of the Apple soft-switches
volatile uint32_t soft_switches;

// Shadow copy of Apple memory locations $0400 - $0BFF
uint8_t text_memory[2*1024];

// Shadow copy of Apple memory locations $2000 - $5FFF
uint8_t hires_memory[2*8192];
