#include "textfont.h"


#ifdef APPLE_MODEL_IIE
#define DEFAULT_CHARACTER_ROM textfont_iie_us_enhanced
const uint8_t *default_character_rom = DEFAULT_CHARACTER_ROM;
const uint8_t *character_roms[16] = {
    textfont_iie_us_enhanced,
    textfont_iie_us_unenhanced,
    textfont_iie_us_reactive,
    textfont_iie_uk_enhanced,
    textfont_iie_fr_ca_enhanced,
    textfont_iie_de_improved,
    textfont_iie_spanish_enhanced,
    textfont_iie_it_enhanced,
    textfont_iie_hebrew_enhanced,
    textfont_iie_se_fi_enhanced,
    DEFAULT_CHARACTER_ROM,
    DEFAULT_CHARACTER_ROM,
    DEFAULT_CHARACTER_ROM,
    DEFAULT_CHARACTER_ROM,
    DEFAULT_CHARACTER_ROM,
    DEFAULT_CHARACTER_ROM,
};
#endif

#ifdef APPLE_MODEL_IIPLUS
#define DEFAULT_CHARACTER_ROM textfont_iiplus_us
const uint8_t *default_character_rom = DEFAULT_CHARACTER_ROM;
const uint8_t *character_roms[16] = {
    textfont_iiplus_us,
    textfont_iiplus_videx_lowercase1,
    textfont_iiplus_videx_lowercase2,
    textfont_iiplus_pigfont,
    textfont_iiplus_jp_katakana,
    DEFAULT_CHARACTER_ROM,
    DEFAULT_CHARACTER_ROM,
    DEFAULT_CHARACTER_ROM,
    DEFAULT_CHARACTER_ROM,
    DEFAULT_CHARACTER_ROM,
    DEFAULT_CHARACTER_ROM,
    DEFAULT_CHARACTER_ROM,
    DEFAULT_CHARACTER_ROM,
    DEFAULT_CHARACTER_ROM,
    DEFAULT_CHARACTER_ROM,
    DEFAULT_CHARACTER_ROM,
};
#endif
