#pragma once

#include <stdint.h>


extern const uint8_t default_character_rom[256 * 8];
#ifdef APPLE_MODEL_IIE
extern const uint8_t alternate_character_rom[256 * 8];
#endif