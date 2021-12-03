#include <string.h>
#include <hardware/pio.h>
#include <hardware/pwm.h>

#include "config.h"
#include "abus.h"
#include "abus.pio.h"


enum {
    ABUS_MAIN_SM = 0,
    ABUS_DEVICE_READ_SM = 1,
};

// TODO pre-processor check to assert that CONFIG_PIN_APPLEBUS_PHI0 == PHI0_GPIO


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
    const enum gpio_function gpfn = (pio == pio0) ? GPIO_FUNC_PIO0 : GPIO_FUNC_PIO1;

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

    // Disable input synchronization on input pins to shave off two clock cycles of input latency
    pio->input_sync_bypass |= (1 << CONFIG_PIN_APPLEBUS_PHI0) | (0x3ff << CONFIG_PIN_APPLEBUS_DATA_BASE);

    gpio_set_function(CONFIG_PIN_APPLEBUS_PHI0, gpfn);
    for(int pin=CONFIG_PIN_APPLEBUS_CONTROL_BASE; pin < CONFIG_PIN_APPLEBUS_CONTROL_BASE+4; pin++) {
        gpio_set_function(pin, gpfn);
        gpio_set_drive_strength(pin, GPIO_DRIVE_STRENGTH_2MA);
    }
    for(int pin=CONFIG_PIN_APPLEBUS_DATA_BASE; pin < CONFIG_PIN_APPLEBUS_DATA_BASE+10; pin++) {
        gpio_set_function(pin, gpfn);
    }
}

void abus_init() {
    abus_device_read_setup(CONFIG_ABUS_PIO, ABUS_DEVICE_READ_SM);
    abus_main_setup(CONFIG_ABUS_PIO, ABUS_MAIN_SM);

    pio_enable_sm_mask_in_sync(CONFIG_ABUS_PIO, (1 << ABUS_MAIN_SM) | (1 << ABUS_DEVICE_READ_SM));
}

void abus_loop() {
    if(0) {
        // Start a square wave generator as a fake Phase 0 testing clock. This must be
        // connected externally to PHI0_GPIO.
        const uint test_gpio = 28;
        const uint pwmSliceNum = pwm_gpio_to_slice_num(test_gpio);
        const uint pwmChannel = pwm_gpio_to_channel(test_gpio);
        gpio_set_function(test_gpio, GPIO_FUNC_PWM);
        pwm_set_clkdiv(pwmSliceNum, 31.5);
        pwm_set_wrap(pwmSliceNum, 3);           // Period of 4 cycles
        pwm_set_chan_level(pwmSliceNum, pwmChannel, 2);  // 50% duty cycle
        pwm_set_enabled(pwmSliceNum, true);
    }

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    uint8_t mem[256];
    memset(mem, 0xff, sizeof(mem));

    uint8_t io[16];
    memset(io, 0xff, sizeof(io));

    const uint mem_mask = 0xff00;
    const uint io_mask = 0xfff0;

    uint mymemprefix = 0;
    uint myioprefix = 0;
    while(1) {
        uint32_t value = pio_sm_get_blocking(CONFIG_ABUS_PIO, ABUS_MAIN_SM);
        uint address = (value >> 10) & 0xffff;

#if 0
        // FIXME: this is all untested, just to see if the CPU has enough time to respond to reads in the worst case
        if((myioprefix == 0) && ((value & (1u << 8)) == 0)) {
            // device is selected for the first time
            myioprefix = address & io_mask;
            mymemprefix = (((address & 0x00f0) - 0x0080) << 8) | 0xc000;
        }

        if((value & (3u << 8)) == (2u << 8)) {
            // read cycle for this device
            uint data;
            if((address & io_mask) == myioprefix) {
                data = io[address & 0xf];
            } else if((address & mem_mask) == mymemprefix) {
                data = mem[address & 0xff];
            } else {
                data = 0xff;
            }
            pio_sm_put(CONFIG_ABUS_PIO, ABUS_DEVICE_READ_SM, data);
        }
#endif
    }
}
