#pragma once

#include <stdint.h>

extern void device_write(uint_fast8_t reg, uint_fast8_t data);
extern void execute_device_command(uint_fast8_t cmd);
