#include "device_regs.h"

#include "buffers.h"
#include "colors.h"


static unsigned int char_write_offset;


// Handle a write to one of the registers on this device's slot
void device_write(uint_fast8_t reg, uint_fast8_t data) {
    switch(reg) {
    case 0x00:
        if(data & 0x01)
            soft_scanline_emulation = true;
        if(data & 0x02)
            soft_scanline_emulation = false;
        break;

    // soft-monochrome color setting
    case 0x01:
        if(data & 0xf) {
            mono_fg_color = mono_fg_colors[data & 0x3];
        }
        if(data & 0xf0) {
            mono_bg_color = mono_bg_colors[(data >> 4) & 0x3];
        }
        break;

    // character generator write offset
    case 0x02:
        char_write_offset = data << 3;
        break;

    // character generator write
    case 0x03:
        character_rom[char_write_offset] = data;
        char_write_offset = (char_write_offset + 1) % sizeof(character_rom);
        break;

    default:;
    }
}
