#include <string.h>
#include <hardware/pio.h>
#include "config.h"
#include "abus.h"
#include "abus.pio.h"
#include "buffers.h"


#if CONFIG_PIN_APPLEBUS_PHI0 != PHI0_GPIO
#error CONFIG_PIN_APPLEBUS_PHI0 and PHI0_GPIO must be set to the same pin
#endif


enum {
    ABUS_MAIN_SM = 0,
    ABUS_DEVICE_READ_SM = 1,
};


static void abus_device_read_setup(PIO pio, uint sm) {
    uint program_offset = pio_add_program(pio, &abus_device_read_program);
    pio_sm_claim(pio, sm);

    pio_sm_config c = abus_device_read_program_get_default_config(program_offset);

    // set the "device selected" pin as the jump pin
    sm_config_set_jmp_pin(&c, CONFIG_PIN_APPLEBUS_DEVSEL);

    // map the OUT pin group to the data signals
    sm_config_set_out_pins(&c, CONFIG_PIN_APPLEBUS_DATA_BASE, 8);

    // map the SET pin group to the Data transceiver control signals
    sm_config_set_set_pins(&c, CONFIG_PIN_APPLEBUS_CONTROL_BASE, 2);

    pio_sm_init(pio, sm, program_offset, &c);

    // All the GPIOs are shared and setup by the main program
}

static void abus_main_setup(PIO pio, uint sm) {
    uint program_offset = pio_add_program(pio, &abus_program);
    pio_sm_claim(pio, sm);

    pio_sm_config c = abus_program_get_default_config(program_offset);

    // set the bus R/W pin as the jump pin
    sm_config_set_jmp_pin(&c, CONFIG_PIN_APPLEBUS_RW);

    // map the IN pin group to the data signals
    sm_config_set_in_pins(&c, CONFIG_PIN_APPLEBUS_DATA_BASE);

    // map the SET pin group to the bus transceiver enable signals
    sm_config_set_set_pins(&c, CONFIG_PIN_APPLEBUS_CONTROL_BASE+1, 3);

    // configure left shift into ISR & autopush every 26 bits
    sm_config_set_in_shift(&c, false, true, 26);

    pio_sm_init(pio, sm, program_offset, &c);

    // configure the GPIOs
    // Ensure all transceivers will start disabled, with Data transceiver direction set to 'in'
    pio_sm_set_pins_with_mask(pio, sm,
        (uint32_t)0xe << CONFIG_PIN_APPLEBUS_CONTROL_BASE,
        (uint32_t)0xf << CONFIG_PIN_APPLEBUS_CONTROL_BASE);
    pio_sm_set_pindirs_with_mask(pio, sm,
        (0xf << CONFIG_PIN_APPLEBUS_CONTROL_BASE),
        (1 << CONFIG_PIN_APPLEBUS_PHI0) | (0xf << CONFIG_PIN_APPLEBUS_CONTROL_BASE) | (0x3ff << CONFIG_PIN_APPLEBUS_DATA_BASE));

    // Disable input synchronization on input pins that are sampled at known stable times
    // to shave off two clock cycles of input latency
    pio->input_sync_bypass |= (0x3ff << CONFIG_PIN_APPLEBUS_DATA_BASE);

    pio_gpio_init(pio, CONFIG_PIN_APPLEBUS_PHI0);
    gpio_set_pulls(CONFIG_PIN_APPLEBUS_PHI0, false, false);
    for(int pin=CONFIG_PIN_APPLEBUS_CONTROL_BASE; pin < CONFIG_PIN_APPLEBUS_CONTROL_BASE+4; pin++) {
        pio_gpio_init(pio, pin);
    }
    for(int pin=CONFIG_PIN_APPLEBUS_DATA_BASE; pin < CONFIG_PIN_APPLEBUS_DATA_BASE+10; pin++) {
        pio_gpio_init(pio, pin);
        gpio_set_pulls(pin, false, false);
    }
}


void abus_init() {
    abus_device_read_setup(CONFIG_ABUS_PIO, ABUS_DEVICE_READ_SM);
    abus_main_setup(CONFIG_ABUS_PIO, ABUS_MAIN_SM);

    pio_enable_sm_mask_in_sync(CONFIG_ABUS_PIO, (1 << ABUS_MAIN_SM) | (1 << ABUS_DEVICE_READ_SM));
}


static void __time_critical_func(shadow_memory)(uint address, uint32_t value) {
    // Shadow parts of the Apple's memory by observing the bus write cycles
    if((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0) {
        if((address >= 0x400u) && (address < 0xc00u)) {
            text_memory[address - 0x400] = value & 0xff;
        } else if((address >= 0x2000) && (address < 0x6000)) {
            hires_memory[address - 0x2000] = value & 0xff;
        }
    }

    // Shadow the soft-switches by observing all read & write bus cycles
    if((address & 0xfff8) == 0xc050) {
        switch(address & 7) {
        case 0:
            soft_switches &= ~((uint32_t)SOFTSW_TEXT_MODE);
            break;
        case 1:
            soft_switches |= SOFTSW_TEXT_MODE;
            break;
        case 2:
            soft_switches &= ~((uint32_t)SOFTSW_MIX_MODE);
            break;
        case 3:
            soft_switches |= SOFTSW_MIX_MODE;
            break;
        case 4:
            soft_switches &= ~((uint32_t)SOFTSW_PAGE_2);
            break;
        case 5:
            soft_switches |= SOFTSW_PAGE_2;
            break;
        case 6:
            soft_switches &= ~((uint32_t)SOFTSW_HIRES_MODE);
            break;
        case 7:
            soft_switches |= SOFTSW_HIRES_MODE;
            break;
        }
    }
}

void __time_critical_func(abus_loop)() {
    while(1) {
        uint32_t value = pio_sm_get_blocking(CONFIG_ABUS_PIO, ABUS_MAIN_SM);
        uint address = (value >> 10) & 0xffff;

        if((value & (1u << CONFIG_PIN_APPLEBUS_DEVSEL-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0) {
            gpio_xor_mask(1u << PICO_DEFAULT_LED_PIN);

            if((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) != 0) {
                // device read access
                pio_sm_put_blocking(CONFIG_ABUS_PIO, ABUS_DEVICE_READ_SM, address & 0xf);
            }
        }

        shadow_memory(address, value);
    }
}
