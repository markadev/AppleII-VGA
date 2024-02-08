#pragma once

void abus_init();
void abus_loop();

#define CARD_SELECT   ((value & (1u << (CONFIG_PIN_APPLEBUS_DEVSEL - CONFIG_PIN_APPLEBUS_DATA_BASE))) == 0)
#define CARD_DEVSEL   ((address & 0xff80) == 0xc080)
#define CARD_IOSEL    (((address & 0xff00) >= 0xc100) && ((address & 0xff00) < 0xc700))
#define CARD_IOSTROBE ((address & 0xf800) == 0xc800)

#define ACCESS_READ  ((value & (1u << (CONFIG_PIN_APPLEBUS_RW - CONFIG_PIN_APPLEBUS_DATA_BASE))) != 0)
#define ACCESS_WRITE ((value & (1u << (CONFIG_PIN_APPLEBUS_RW - CONFIG_PIN_APPLEBUS_DATA_BASE))) == 0)

enum {
    ABUS_MAIN_SM = 0,
    ABUS_DEVICE_READ_SM = 1,
};
