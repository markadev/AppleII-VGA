#include "config.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <hardware/flash.h>
#include "buffers.h"
#include "colors.h"
#include "textfont/textfont.h"
#ifdef APPLE_MODEL_IIPLUS
#include "videx_vterm.h"
#endif


// A block of flash is reserved for storing configuration persistently across power cycles
// and firmware updates.
//
// The memory is divided as:
//  * 4K for a 'config' structure
//  * 16*2K blocks for saved character ROMs
//  * the remaining is reserved for future use


#define MAGIC_WORD_VALUE 0x56474131

struct config {
    // the real size of the stored structure
    uint16_t size;

    uint8_t scanline_emulation;
    uint8_t monochrome;
    uint16_t mono_bg_color;
    uint16_t mono_fg_color;
    uint8_t character_rom[CHARACTER_ROM_SIZE];

    // magic word determines if the stored configuration is valid
    uint32_t magic_word;

    // Add new fields after here. When reading the config use the IS_STORED_IN_CONFIG macro
    // to determine if the field you're looking for is actually present in the stored config.

    uint8_t videx_vterm_enabled;
    uint8_t force_alt_textcolor;
};

// This is a compile-time check to ensure the size of the config struct fits within one flash erase sector
typedef char config_struct_size_check[(sizeof(struct config) <= FLASH_SECTOR_SIZE) - 1];

#define IS_STORED_IN_CONFIG(cfg, field) ((offsetof(struct config, field) + sizeof((cfg)->field)) <= (cfg)->size)


extern uint8_t __persistent_data_start[];
static struct config *cfg = (struct config *)__persistent_data_start;
// TODO static uint8_t *character_rom_storage = __persistent_data_start + FLASH_SECTOR_SIZE;


void config_load() {
    if((cfg->magic_word != MAGIC_WORD_VALUE) || (cfg->size > FLASH_SECTOR_SIZE)) {
        config_load_defaults();
        return;
    }

    soft_scanline_emulation = cfg->scanline_emulation;
    soft_monochrom = cfg->monochrome;
    mono_bg_color = cfg->mono_bg_color;
    mono_fg_color = cfg->mono_fg_color;
    memcpy(character_rom, cfg->character_rom, CHARACTER_ROM_SIZE);

#ifdef APPLE_MODEL_IIPLUS
    if(IS_STORED_IN_CONFIG(cfg, videx_vterm_enabled) && cfg->videx_vterm_enabled) {
        videx_vterm_enable();
    } else {
        videx_vterm_disable();
    }
#endif

    soft_force_alt_textcolor = IS_STORED_IN_CONFIG(cfg, force_alt_textcolor) ? cfg->force_alt_textcolor : false;
}


void config_load_defaults() {
    soft_scanline_emulation = false;
    soft_monochrom = false;
    mono_bg_color = mono_bg_colors[1];
    mono_fg_color = mono_fg_colors[1];
    memcpy(character_rom, default_character_rom, CHARACTER_ROM_SIZE);
#ifdef APPLE_MODEL_IIPLUS
    videx_vterm_disable();
#endif
    soft_force_alt_textcolor = false;
}


void config_save() {
    // the write buffer size must be a multiple of FLASH_PAGE_SIZE so round up
    const int new_config_size = (sizeof(struct config) + FLASH_PAGE_SIZE - 1) & -FLASH_PAGE_SIZE;
    struct config *new_config = malloc(new_config_size);
    memset(new_config, 0xff, new_config_size);
    memset(new_config, 0, sizeof(struct config));

    new_config->size = sizeof(struct config);
    new_config->scanline_emulation = soft_scanline_emulation;
    new_config->monochrome = soft_monochrom;
    new_config->mono_bg_color = mono_bg_color;
    new_config->mono_fg_color = mono_fg_color;
    memcpy(new_config->character_rom, character_rom, CHARACTER_ROM_SIZE);
    new_config->magic_word = MAGIC_WORD_VALUE;
#ifdef APPLE_MODEL_IIPLUS
    new_config->videx_vterm_enabled = videx_vterm_enabled;
#endif
    new_config->force_alt_textcolor = soft_force_alt_textcolor;

    const uint32_t flash_offset = (uint32_t)cfg - XIP_BASE;
    flash_range_erase(flash_offset, FLASH_SECTOR_SIZE);
    flash_range_program(flash_offset, (uint8_t *)new_config, new_config_size);

    free(new_config);
}
