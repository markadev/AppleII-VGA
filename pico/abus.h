#pragma once

void abus_init();
void abus_loop();

#define ACCESS_READ  ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) != 0)
#define ACCESS_WRITE ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)
