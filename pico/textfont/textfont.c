#include "textfont.h"


#ifdef APPLE_MODEL_IIE
#define DEFAULT_CHARACTER_ROM textfont_iie_us_enhanced
const uint8_t *default_character_rom = DEFAULT_CHARACTER_ROM;
const uint8_t *character_roms[16] = {
    textfont_iie_us_enhanced,
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
};
#endif

#ifdef APPLE_MODEL_IIPLUS
#define DEFAULT_CHARACTER_ROM textfont_iiplus_us
const uint8_t *default_character_rom = DEFAULT_CHARACTER_ROM;
const uint8_t *character_roms[16] = {
    textfont_iiplus_us, textfont_iiplus_videx_lowercase1,
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
    DEFAULT_CHARACTER_ROM,  // slot available for use
};
#endif
