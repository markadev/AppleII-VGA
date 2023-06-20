#pragma once

#include <stdint.h>

// Mapping of a hires video byte to 14 half-pixel dots.
// Bits are displayed from MSB to LSB.
static uint16_t hires_dot_patterns[256] = {
    0x0000,0x3000,0x0c00,0x3c00,0x0300,0x3300,0x0f00,0x3f00,
    0x00c0,0x30c0,0x0cc0,0x3cc0,0x03c0,0x33c0,0x0fc0,0x3fc0,
    0x0030,0x3030,0x0c30,0x3c30,0x0330,0x3330,0x0f30,0x3f30,
    0x00f0,0x30f0,0x0cf0,0x3cf0,0x03f0,0x33f0,0x0ff0,0x3ff0,
    0x000c,0x300c,0x0c0c,0x3c0c,0x030c,0x330c,0x0f0c,0x3f0c,
    0x00cc,0x30cc,0x0ccc,0x3ccc,0x03cc,0x33cc,0x0fcc,0x3fcc,
    0x003c,0x303c,0x0c3c,0x3c3c,0x033c,0x333c,0x0f3c,0x3f3c,
    0x00fc,0x30fc,0x0cfc,0x3cfc,0x03fc,0x33fc,0x0ffc,0x3ffc,
    0x0003,0x3003,0x0c03,0x3c03,0x0303,0x3303,0x0f03,0x3f03,
    0x00c3,0x30c3,0x0cc3,0x3cc3,0x03c3,0x33c3,0x0fc3,0x3fc3,
    0x0033,0x3033,0x0c33,0x3c33,0x0333,0x3333,0x0f33,0x3f33,
    0x00f3,0x30f3,0x0cf3,0x3cf3,0x03f3,0x33f3,0x0ff3,0x3ff3,
    0x000f,0x300f,0x0c0f,0x3c0f,0x030f,0x330f,0x0f0f,0x3f0f,
    0x00cf,0x30cf,0x0ccf,0x3ccf,0x03cf,0x33cf,0x0fcf,0x3fcf,
    0x003f,0x303f,0x0c3f,0x3c3f,0x033f,0x333f,0x0f3f,0x3f3f,
    0x00ff,0x30ff,0x0cff,0x3cff,0x03ff,0x33ff,0x0fff,0x3fff,
    0x0000,0x1800,0x0600,0x1e00,0x0180,0x1980,0x0780,0x1f80,
    0x0060,0x1860,0x0660,0x1e60,0x01e0,0x19e0,0x07e0,0x1fe0,
    0x0018,0x1818,0x0618,0x1e18,0x0198,0x1998,0x0798,0x1f98,
    0x0078,0x1878,0x0678,0x1e78,0x01f8,0x19f8,0x07f8,0x1ff8,
    0x0006,0x1806,0x0606,0x1e06,0x0186,0x1986,0x0786,0x1f86,
    0x0066,0x1866,0x0666,0x1e66,0x01e6,0x19e6,0x07e6,0x1fe6,
    0x001e,0x181e,0x061e,0x1e1e,0x019e,0x199e,0x079e,0x1f9e,
    0x007e,0x187e,0x067e,0x1e7e,0x01fe,0x19fe,0x07fe,0x1ffe,
    0x0001,0x1801,0x0601,0x1e01,0x0181,0x1981,0x0781,0x1f81,
    0x0061,0x1861,0x0661,0x1e61,0x01e1,0x19e1,0x07e1,0x1fe1,
    0x0019,0x1819,0x0619,0x1e19,0x0199,0x1999,0x0799,0x1f99,
    0x0079,0x1879,0x0679,0x1e79,0x01f9,0x19f9,0x07f9,0x1ff9,
    0x0007,0x1807,0x0607,0x1e07,0x0187,0x1987,0x0787,0x1f87,
    0x0067,0x1867,0x0667,0x1e67,0x01e7,0x19e7,0x07e7,0x1fe7,
    0x001f,0x181f,0x061f,0x1e1f,0x019f,0x199f,0x079f,0x1f9f,
    0x007f,0x187f,0x067f,0x1e7f,0x01ff,0x19ff,0x07ff,0x1fff,
};

uint8_t dgr_dot_pattern[32] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x08, 0x19, 0x2A, 0x3B, 0x4C, 0x5D, 0x6E, 0x7F,
    0x00, 0x44, 0x08, 0x4C, 0x11, 0x55, 0x19, 0x5D, 0x22, 0x66, 0x2A, 0x6E, 0x33, 0x77, 0x3B, 0x7F,
};

uint8_t dgr_dot_pattern[32] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x08, 0x19, 0x2A, 0x3B, 0x4C, 0x5D, 0x6E, 0x7F,
    0x00, 0x44, 0x08, 0x4C, 0x11, 0x55, 0x19, 0x5D, 0x22, 0x66, 0x2A, 0x6E, 0x33, 0x77, 0x3B, 0x7F,
};
