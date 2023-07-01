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
    sm_config_set_set_pins(&c, CONFIG_PIN_APPLEBUS_CONTROL_BASE + 1, 3);

    // configure left shift into ISR & autopush every 26 bits
    sm_config_set_in_shift(&c, false, true, 26);

    pio_sm_init(pio, sm, program_offset, &c);

    // configure the GPIOs
    // Ensure all transceivers will start disabled, with Data transceiver direction set to 'in'
    pio_sm_set_pins_with_mask(
        pio, sm, (uint32_t)0xe << CONFIG_PIN_APPLEBUS_CONTROL_BASE, (uint32_t)0xf << CONFIG_PIN_APPLEBUS_CONTROL_BASE);
    pio_sm_set_pindirs_with_mask(pio, sm, (0xf << CONFIG_PIN_APPLEBUS_CONTROL_BASE),
        (1 << CONFIG_PIN_APPLEBUS_PHI0) | (0xf << CONFIG_PIN_APPLEBUS_CONTROL_BASE) | (0x3ff << CONFIG_PIN_APPLEBUS_DATA_BASE));

    // Disable input synchronization on input pins that are sampled at known stable times
    // to shave off two clock cycles of input latency
    pio->input_sync_bypass |= (0x3ff << CONFIG_PIN_APPLEBUS_DATA_BASE);

    pio_gpio_init(pio, CONFIG_PIN_APPLEBUS_PHI0);
    gpio_set_pulls(CONFIG_PIN_APPLEBUS_PHI0, false, false);

    for(int pin = CONFIG_PIN_APPLEBUS_CONTROL_BASE; pin < CONFIG_PIN_APPLEBUS_CONTROL_BASE + 4; pin++) {
        pio_gpio_init(pio, pin);
    }

    for(int pin = CONFIG_PIN_APPLEBUS_DATA_BASE; pin < CONFIG_PIN_APPLEBUS_DATA_BASE + 10; pin++) {
        pio_gpio_init(pio, pin);
        gpio_set_pulls(pin, false, false);
    }
}


void abus_init() {
    //! Init states
    soft_switches = SOFTSW_TEXT_MODE;
    soft_ramwrt = 0;
    soft_80col = 0;
    soft_80store = 0;
    soft_video7 = VIDEO7_MODE0;
    soft_dhires = 0;
    soft_monochrom = 0;
    memset(private_memory, 0x00, 64 * 1024);
    memset(main_memory, 0x00, 64 * 1024);
    //! put empty text
    memset(main_memory + 0x400, 0x20, 2 * 1024);
    memset(private_memory + 0x400, 0x20, 2 * 1024);

    abus_device_read_setup(CONFIG_ABUS_PIO, ABUS_DEVICE_READ_SM);
    abus_main_setup(CONFIG_ABUS_PIO, ABUS_MAIN_SM);

    pio_enable_sm_mask_in_sync(CONFIG_ABUS_PIO, (1 << ABUS_MAIN_SM) | (1 << ABUS_DEVICE_READ_SM));
}


static void __time_critical_func(shadow_memory)(uint address, uint32_t value) {
    static int bit0_set = 0;
    // Shadow parts of the Apple's memory by observing the bus write cycles
    static bool reset_phase_1_happening = false;

    //! Reset Detection
    if((address == 0xFFFC) && (ACCESS_READ)) {
        reset_phase_1_happening = true;
    } else if((address == 0xFFFD) && (ACCESS_READ) && (reset_phase_1_happening)) {
        //! Reset Clear all buffers
        soft_switches = SOFTSW_TEXT_MODE;
        soft_ramwrt = 0;
        soft_80col = 0;
        soft_80store = 0;
        soft_video7 = VIDEO7_MODE0;
        bit0_set = 0;
        soft_dhires = 0;
        soft_monochrom = 0;
        reset_phase_1_happening = false;
    } else {
        reset_phase_1_happening = false;
    }

    if(ACCESS_WRITE) {
        // Mirror Video Memory from MAIN & AUX banks
        if(soft_80store) {
            if(soft_switches & SOFTSW_PAGE_2) {
                if((address >= 0x400) && (address < 0x800)) {
                    private_memory[address] = value & 0xff;
                    return;
                } else if((soft_switches & SOFTSW_HIRES_MODE) && (address >= 0x2000) && (address < 0x4000)) {
                    private_memory[address] = value & 0xff;
                    return;
                }
            }
        } else if(soft_ramwrt) {
            if((address >= 0x200) && (address < 0xC000)) {
                private_memory[address] = value & 0xff;
                return;
            }
        }

        if((address >= 0x200) && (address < 0xC000)) {
            main_memory[address] = value & 0xff;
            return;
        }
    }

    if(address < 0xc000) {
        return;
    }

    if(address < 0xc080) {
#ifdef APPLE_MODEL_IIE
        //! Soft switches
        // Shadow the soft-switches by observing all read & write bus cycles
        if(address == 0xC000 && ACCESS_WRITE) {
            soft_80store = ((uint32_t)SOFTSW_80STORE_OFF);
        }

        if(address == 0xc001 && ACCESS_WRITE) {
            soft_80store = ((uint32_t)SOFTSW_80STORE_ON);
        }

        if(address == 0xc002 && ACCESS_WRITE) {
            soft_ramrd = ((uint32_t)SOFTSW_READ_MAIN);
        }

        if(address == 0xc003 && ACCESS_WRITE) {
            soft_ramrd = ((uint32_t)SOFTSW_READ_AUX);
        }

        if(address == 0xc004 && ACCESS_WRITE) {
            soft_ramwrt = ((uint32_t)SOFTSW_WRITE_MAIN);
        }

        if(address == 0xc005 && ACCESS_WRITE) {
            soft_ramwrt = ((uint32_t)SOFTSW_WRITE_AUX);
        }

        if(address == 0xc00c && ACCESS_WRITE) {
            soft_80col = ((uint32_t)SOFTSW_80COL_OFF);
        }

        if(address == 0xc00e && ACCESS_WRITE) {
            soft_switches_alt = ((uint32_t)SOFTSW_ALTCHAR_OFF);
        }

        if(address == 0xc00f && ACCESS_WRITE) {
            soft_switches_alt = ((uint32_t)SOFTSW_ALTCHAR_ON);
        }

        if(address == 0xc00d && ACCESS_WRITE) {
            soft_80col = ((uint32_t)SOFTSW_80COL_ON);
        }

        if(address == 0xc07e && ACCESS_WRITE) {
            soft_ioudis = ((uint32_t)SOFTSW_IOUDIS_ON);
        }

        if(address == 0xc07f && ACCESS_WRITE) {
            soft_ioudis = ((uint32_t)SOFTSW_IOUDIS_OFF);
        }

        if(address == 0xc05e) {
            soft_an3 = ((uint32_t)SOFTSW_AN3_OFF);
            soft_dhires = ((uint32_t)SOFTSW_DHIRES_ON);
            return;
        }

        if(address == 0xc05f) {
            if(soft_dhires) {
                //! This is the VIDEO7 Magic (Not documented by apple but by a patent US4631692 )
                //! Apple ii has softswitches and also a special 2bit shift register (two flipflops basicly)
                //! controlled with Softwitch 80COL and AN3, AN3 is the Clock, when AN3 goes from clear to set it puts
                //! the content of 80COL in the 2 switches
                //! this is VIDEO7 Mode

                if(!bit0_set) {
                    soft_video7 = (0x01) & (soft_video7 | !soft_80col);
                    bit0_set = 1;
                } else {
                    soft_video7 = soft_video7 | ((!soft_80col) << 1);
                    //! reset state
                    bit0_set = 0;
                }
            }

            soft_an3 = ((uint32_t)SOFTSW_AN3_ON);
            soft_dhires = ((uint32_t)SOFTSW_DHIRES_OFF);
            return;
        }
#endif
        if(address == 0xC021 && ACCESS_WRITE) {
            if(value & 0x80) {
                soft_monochrom = SOFTSW_MONO_EN;
            } else {
                soft_monochrom = SOFTSW_MONO_DIS;
            }
        }

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
        return;
    }
}


void __time_critical_func(abus_loop)() {
    while(1) {
        uint32_t value = pio_sm_get_blocking(CONFIG_ABUS_PIO, ABUS_MAIN_SM);
        uint address = (value >> 10) & 0xffff;

        if(DESVEL) {
            if(ACCESS_READ) {
                // device read access
                pio_sm_put_blocking(CONFIG_ABUS_PIO, ABUS_DEVICE_READ_SM, address & 0xf);
            }
            gpio_xor_mask(1u << PICO_DEFAULT_LED_PIN);
        }

        shadow_memory(address, value);
    }
}
