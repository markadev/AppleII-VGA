#pragma once

#define _PIXELS(color1, color2) ((uint32_t)(color1) | ((uint32_t)(color2) << 16))
static uint32_t hires_color_patterns[2*256] = {
    // Even byte pixels
    /* 0b00000000 */ _PIXELS(0x000, 0x000),
    /* 0b00000001 */ _PIXELS(0x000, 0x000),
    /* 0b00000010 */ _PIXELS(0x000, 0x000),
    /* 0b00000011 */ _PIXELS(0x000, 0x000),
    /* 0b00000100 */ _PIXELS(0x000, 0x000),
    /* 0b00000101 */ _PIXELS(0x000, 0x000),
    /* 0b00000110 */ _PIXELS(0x000, 0x000),
    /* 0b00000111 */ _PIXELS(0x000, 0x000),
    /* 0b00001000 */ _PIXELS(0x000, 0x04d),
    /* 0b00001001 */ _PIXELS(0x000, 0x10f),
    /* 0b00001010 */ _PIXELS(0x000, 0x124),
    /* 0b00001011 */ _PIXELS(0x000, 0x1de),
    /* 0b00001100 */ _PIXELS(0x000, 0x01d),
    /* 0b00001101 */ _PIXELS(0x000, 0x1b7),
    /* 0b00001110 */ _PIXELS(0x000, 0x0fd),
    /* 0b00001111 */ _PIXELS(0x000, 0x1ff),
    /* 0b00010000 */ _PIXELS(0x102, 0x000),
    /* 0b00010001 */ _PIXELS(0x102, 0x102),
    /* 0b00010010 */ _PIXELS(0x150, 0x150),
    /* 0b00010011 */ _PIXELS(0x150, 0x150),
    /* 0b00010100 */ _PIXELS(0x0db, 0x0db),
    /* 0b00010101 */ _PIXELS(0x0db, 0x0db),
    /* 0b00010110 */ _PIXELS(0x120, 0x120),
    /* 0b00010111 */ _PIXELS(0x120, 0x120),
    /* 0b00011000 */ _PIXELS(0x10f, 0x10f),
    /* 0b00011001 */ _PIXELS(0x10f, 0x10f),
    /* 0b00011010 */ _PIXELS(0x1de, 0x1de),
    /* 0b00011011 */ _PIXELS(0x1de, 0x1de),
    /* 0b00011100 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b00011101 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b00011110 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b00011111 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b00100000 */ _PIXELS(0x000, 0x000),
    /* 0b00100001 */ _PIXELS(0x000, 0x000),
    /* 0b00100010 */ _PIXELS(0x090, 0x090),
    /* 0b00100011 */ _PIXELS(0x090, 0x150),
    /* 0b00100100 */ _PIXELS(0x019, 0x019),
    /* 0b00100101 */ _PIXELS(0x019, 0x0db),
    /* 0b00100110 */ _PIXELS(0x060, 0x060),
    /* 0b00100111 */ _PIXELS(0x060, 0x120),
    /* 0b00101000 */ _PIXELS(0x04d, 0x04d),
    /* 0b00101001 */ _PIXELS(0x04d, 0x10f),
    /* 0b00101010 */ _PIXELS(0x124, 0x124),
    /* 0b00101011 */ _PIXELS(0x124, 0x1de),
    /* 0b00101100 */ _PIXELS(0x01d, 0x01d),
    /* 0b00101101 */ _PIXELS(0x01d, 0x1b7),
    /* 0b00101110 */ _PIXELS(0x0fd, 0x0fd),
    /* 0b00101111 */ _PIXELS(0x0fd, 0x1ff),
    /* 0b00110000 */ _PIXELS(0x102, 0x000),
    /* 0b00110001 */ _PIXELS(0x102, 0x102),
    /* 0b00110010 */ _PIXELS(0x150, 0x150),
    /* 0b00110011 */ _PIXELS(0x150, 0x150),
    /* 0b00110100 */ _PIXELS(0x0db, 0x0db),
    /* 0b00110101 */ _PIXELS(0x0db, 0x0db),
    /* 0b00110110 */ _PIXELS(0x120, 0x120),
    /* 0b00110111 */ _PIXELS(0x120, 0x120),
    /* 0b00111000 */ _PIXELS(0x10f, 0x10f),
    /* 0b00111001 */ _PIXELS(0x10f, 0x10f),
    /* 0b00111010 */ _PIXELS(0x1de, 0x1de),
    /* 0b00111011 */ _PIXELS(0x1de, 0x1de),
    /* 0b00111100 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b00111101 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b00111110 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b00111111 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01000000 */ _PIXELS(0x000, 0x000),
    /* 0b01000001 */ _PIXELS(0x000, 0x000),
    /* 0b01000010 */ _PIXELS(0x000, 0x000),
    /* 0b01000011 */ _PIXELS(0x000, 0x000),
    /* 0b01000100 */ _PIXELS(0x019, 0x019),
    /* 0b01000101 */ _PIXELS(0x019, 0x0db),
    /* 0b01000110 */ _PIXELS(0x060, 0x060),
    /* 0b01000111 */ _PIXELS(0x060, 0x120),
    /* 0b01001000 */ _PIXELS(0x04d, 0x04d),
    /* 0b01001001 */ _PIXELS(0x04d, 0x10f),
    /* 0b01001010 */ _PIXELS(0x124, 0x124),
    /* 0b01001011 */ _PIXELS(0x124, 0x1de),
    /* 0b01001100 */ _PIXELS(0x01d, 0x01d),
    /* 0b01001101 */ _PIXELS(0x01d, 0x1b7),
    /* 0b01001110 */ _PIXELS(0x0fd, 0x0fd),
    /* 0b01001111 */ _PIXELS(0x0fd, 0x1ff),
    /* 0b01010000 */ _PIXELS(0x102, 0x000),
    /* 0b01010001 */ _PIXELS(0x102, 0x102),
    /* 0b01010010 */ _PIXELS(0x150, 0x150),
    /* 0b01010011 */ _PIXELS(0x150, 0x150),
    /* 0b01010100 */ _PIXELS(0x0db, 0x0db),
    /* 0b01010101 */ _PIXELS(0x0db, 0x0db),
    /* 0b01010110 */ _PIXELS(0x120, 0x120),
    /* 0b01010111 */ _PIXELS(0x120, 0x120),
    /* 0b01011000 */ _PIXELS(0x10f, 0x10f),
    /* 0b01011001 */ _PIXELS(0x10f, 0x10f),
    /* 0b01011010 */ _PIXELS(0x1de, 0x1de),
    /* 0b01011011 */ _PIXELS(0x1de, 0x1de),
    /* 0b01011100 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b01011101 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b01011110 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01011111 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01100000 */ _PIXELS(0x000, 0x000),
    /* 0b01100001 */ _PIXELS(0x000, 0x000),
    /* 0b01100010 */ _PIXELS(0x090, 0x090),
    /* 0b01100011 */ _PIXELS(0x090, 0x150),
    /* 0b01100100 */ _PIXELS(0x019, 0x019),
    /* 0b01100101 */ _PIXELS(0x019, 0x0db),
    /* 0b01100110 */ _PIXELS(0x060, 0x060),
    /* 0b01100111 */ _PIXELS(0x060, 0x120),
    /* 0b01101000 */ _PIXELS(0x04d, 0x04d),
    /* 0b01101001 */ _PIXELS(0x04d, 0x10f),
    /* 0b01101010 */ _PIXELS(0x124, 0x124),
    /* 0b01101011 */ _PIXELS(0x124, 0x1de),
    /* 0b01101100 */ _PIXELS(0x01d, 0x01d),
    /* 0b01101101 */ _PIXELS(0x01d, 0x1b7),
    /* 0b01101110 */ _PIXELS(0x0fd, 0x0fd),
    /* 0b01101111 */ _PIXELS(0x0fd, 0x1ff),
    /* 0b01110000 */ _PIXELS(0x102, 0x000),
    /* 0b01110001 */ _PIXELS(0x102, 0x102),
    /* 0b01110010 */ _PIXELS(0x150, 0x150),
    /* 0b01110011 */ _PIXELS(0x150, 0x150),
    /* 0b01110100 */ _PIXELS(0x0db, 0x0db),
    /* 0b01110101 */ _PIXELS(0x0db, 0x0db),
    /* 0b01110110 */ _PIXELS(0x120, 0x120),
    /* 0b01110111 */ _PIXELS(0x120, 0x120),
    /* 0b01111000 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01111001 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01111010 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01111011 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01111100 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01111101 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01111110 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01111111 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b10000000 */ _PIXELS(0x000, 0x000),
    /* 0b10000001 */ _PIXELS(0x000, 0x000),
    /* 0b10000010 */ _PIXELS(0x000, 0x000),
    /* 0b10000011 */ _PIXELS(0x000, 0x000),
    /* 0b10000100 */ _PIXELS(0x000, 0x000),
    /* 0b10000101 */ _PIXELS(0x000, 0x000),
    /* 0b10000110 */ _PIXELS(0x000, 0x000),
    /* 0b10000111 */ _PIXELS(0x000, 0x000),
    /* 0b10001000 */ _PIXELS(0x04d, 0x04d),
    /* 0b10001001 */ _PIXELS(0x04d, 0x10f),
    /* 0b10001010 */ _PIXELS(0x124, 0x124),
    /* 0b10001011 */ _PIXELS(0x124, 0x1de),
    /* 0b10001100 */ _PIXELS(0x01d, 0x01d),
    /* 0b10001101 */ _PIXELS(0x01d, 0x1b7),
    /* 0b10001110 */ _PIXELS(0x0fd, 0x0fd),
    /* 0b10001111 */ _PIXELS(0x0fd, 0x1ff),
    /* 0b10010000 */ _PIXELS(0x102, 0x000),
    /* 0b10010001 */ _PIXELS(0x102, 0x102),
    /* 0b10010010 */ _PIXELS(0x150, 0x150),
    /* 0b10010011 */ _PIXELS(0x150, 0x150),
    /* 0b10010100 */ _PIXELS(0x0db, 0x0db),
    /* 0b10010101 */ _PIXELS(0x0db, 0x0db),
    /* 0b10010110 */ _PIXELS(0x120, 0x120),
    /* 0b10010111 */ _PIXELS(0x120, 0x120),
    /* 0b10011000 */ _PIXELS(0x10f, 0x10f),
    /* 0b10011001 */ _PIXELS(0x10f, 0x10f),
    /* 0b10011010 */ _PIXELS(0x1de, 0x1de),
    /* 0b10011011 */ _PIXELS(0x1de, 0x1de),
    /* 0b10011100 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b10011101 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b10011110 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b10011111 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b10100000 */ _PIXELS(0x000, 0x000),
    /* 0b10100001 */ _PIXELS(0x000, 0x000),
    /* 0b10100010 */ _PIXELS(0x090, 0x090),
    /* 0b10100011 */ _PIXELS(0x090, 0x150),
    /* 0b10100100 */ _PIXELS(0x019, 0x019),
    /* 0b10100101 */ _PIXELS(0x019, 0x0db),
    /* 0b10100110 */ _PIXELS(0x060, 0x060),
    /* 0b10100111 */ _PIXELS(0x060, 0x120),
    /* 0b10101000 */ _PIXELS(0x04d, 0x04d),
    /* 0b10101001 */ _PIXELS(0x04d, 0x10f),
    /* 0b10101010 */ _PIXELS(0x124, 0x124),
    /* 0b10101011 */ _PIXELS(0x124, 0x1de),
    /* 0b10101100 */ _PIXELS(0x01d, 0x01d),
    /* 0b10101101 */ _PIXELS(0x01d, 0x1b7),
    /* 0b10101110 */ _PIXELS(0x0fd, 0x0fd),
    /* 0b10101111 */ _PIXELS(0x0fd, 0x1ff),
    /* 0b10110000 */ _PIXELS(0x102, 0x000),
    /* 0b10110001 */ _PIXELS(0x102, 0x102),
    /* 0b10110010 */ _PIXELS(0x150, 0x150),
    /* 0b10110011 */ _PIXELS(0x150, 0x150),
    /* 0b10110100 */ _PIXELS(0x0db, 0x0db),
    /* 0b10110101 */ _PIXELS(0x0db, 0x0db),
    /* 0b10110110 */ _PIXELS(0x120, 0x120),
    /* 0b10110111 */ _PIXELS(0x120, 0x120),
    /* 0b10111000 */ _PIXELS(0x10f, 0x10f),
    /* 0b10111001 */ _PIXELS(0x10f, 0x10f),
    /* 0b10111010 */ _PIXELS(0x1de, 0x1de),
    /* 0b10111011 */ _PIXELS(0x1de, 0x1de),
    /* 0b10111100 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b10111101 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b10111110 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b10111111 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11000000 */ _PIXELS(0x000, 0x000),
    /* 0b11000001 */ _PIXELS(0x000, 0x000),
    /* 0b11000010 */ _PIXELS(0x000, 0x000),
    /* 0b11000011 */ _PIXELS(0x000, 0x000),
    /* 0b11000100 */ _PIXELS(0x019, 0x019),
    /* 0b11000101 */ _PIXELS(0x019, 0x0db),
    /* 0b11000110 */ _PIXELS(0x060, 0x060),
    /* 0b11000111 */ _PIXELS(0x060, 0x120),
    /* 0b11001000 */ _PIXELS(0x04d, 0x04d),
    /* 0b11001001 */ _PIXELS(0x04d, 0x10f),
    /* 0b11001010 */ _PIXELS(0x124, 0x124),
    /* 0b11001011 */ _PIXELS(0x124, 0x1de),
    /* 0b11001100 */ _PIXELS(0x01d, 0x01d),
    /* 0b11001101 */ _PIXELS(0x01d, 0x1b7),
    /* 0b11001110 */ _PIXELS(0x0fd, 0x0fd),
    /* 0b11001111 */ _PIXELS(0x0fd, 0x1ff),
    /* 0b11010000 */ _PIXELS(0x102, 0x000),
    /* 0b11010001 */ _PIXELS(0x102, 0x102),
    /* 0b11010010 */ _PIXELS(0x150, 0x150),
    /* 0b11010011 */ _PIXELS(0x150, 0x150),
    /* 0b11010100 */ _PIXELS(0x0db, 0x0db),
    /* 0b11010101 */ _PIXELS(0x0db, 0x0db),
    /* 0b11010110 */ _PIXELS(0x120, 0x120),
    /* 0b11010111 */ _PIXELS(0x120, 0x120),
    /* 0b11011000 */ _PIXELS(0x10f, 0x10f),
    /* 0b11011001 */ _PIXELS(0x10f, 0x10f),
    /* 0b11011010 */ _PIXELS(0x1de, 0x1de),
    /* 0b11011011 */ _PIXELS(0x1de, 0x1de),
    /* 0b11011100 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b11011101 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b11011110 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11011111 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11100000 */ _PIXELS(0x000, 0x000),
    /* 0b11100001 */ _PIXELS(0x000, 0x000),
    /* 0b11100010 */ _PIXELS(0x090, 0x090),
    /* 0b11100011 */ _PIXELS(0x090, 0x150),
    /* 0b11100100 */ _PIXELS(0x019, 0x019),
    /* 0b11100101 */ _PIXELS(0x019, 0x0db),
    /* 0b11100110 */ _PIXELS(0x060, 0x060),
    /* 0b11100111 */ _PIXELS(0x060, 0x120),
    /* 0b11101000 */ _PIXELS(0x04d, 0x04d),
    /* 0b11101001 */ _PIXELS(0x04d, 0x10f),
    /* 0b11101010 */ _PIXELS(0x124, 0x124),
    /* 0b11101011 */ _PIXELS(0x124, 0x1de),
    /* 0b11101100 */ _PIXELS(0x01d, 0x01d),
    /* 0b11101101 */ _PIXELS(0x01d, 0x1b7),
    /* 0b11101110 */ _PIXELS(0x0fd, 0x0fd),
    /* 0b11101111 */ _PIXELS(0x0fd, 0x1ff),
    /* 0b11110000 */ _PIXELS(0x1ff, 0x000),
    /* 0b11110001 */ _PIXELS(0x1ff, 0x102),
    /* 0b11110010 */ _PIXELS(0x1ff, 0x150),
    /* 0b11110011 */ _PIXELS(0x1ff, 0x150),
    /* 0b11110100 */ _PIXELS(0x1ff, 0x0db),
    /* 0b11110101 */ _PIXELS(0x1ff, 0x0db),
    /* 0b11110110 */ _PIXELS(0x1ff, 0x120),
    /* 0b11110111 */ _PIXELS(0x1ff, 0x120),
    /* 0b11111000 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11111001 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11111010 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11111011 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11111100 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11111101 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11111110 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11111111 */ _PIXELS(0x1ff, 0x1ff),

    // Odd byte pixels
    /* 0b00000000 */ _PIXELS(0x000, 0x000),
    /* 0b00000001 */ _PIXELS(0x000, 0x000),
    /* 0b00000010 */ _PIXELS(0x000, 0x000),
    /* 0b00000011 */ _PIXELS(0x000, 0x000),
    /* 0b00000100 */ _PIXELS(0x000, 0x000),
    /* 0b00000101 */ _PIXELS(0x000, 0x000),
    /* 0b00000110 */ _PIXELS(0x000, 0x000),
    /* 0b00000111 */ _PIXELS(0x000, 0x000),
    /* 0b00001000 */ _PIXELS(0x000, 0x090),
    /* 0b00001001 */ _PIXELS(0x000, 0x060),
    /* 0b00001010 */ _PIXELS(0x000, 0x124),
    /* 0b00001011 */ _PIXELS(0x000, 0x0fd),
    /* 0b00001100 */ _PIXELS(0x000, 0x150),
    /* 0b00001101 */ _PIXELS(0x000, 0x120),
    /* 0b00001110 */ _PIXELS(0x000, 0x1de),
    /* 0b00001111 */ _PIXELS(0x000, 0x1ff),
    /* 0b00010000 */ _PIXELS(0x019, 0x000),
    /* 0b00010001 */ _PIXELS(0x019, 0x019),
    /* 0b00010010 */ _PIXELS(0x01d, 0x01d),
    /* 0b00010011 */ _PIXELS(0x01d, 0x01d),
    /* 0b00010100 */ _PIXELS(0x0db, 0x0db),
    /* 0b00010101 */ _PIXELS(0x0db, 0x0db),
    /* 0b00010110 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b00010111 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b00011000 */ _PIXELS(0x060, 0x060),
    /* 0b00011001 */ _PIXELS(0x060, 0x060),
    /* 0b00011010 */ _PIXELS(0x0fd, 0x0fd),
    /* 0b00011011 */ _PIXELS(0x0fd, 0x0fd),
    /* 0b00011100 */ _PIXELS(0x120, 0x120),
    /* 0b00011101 */ _PIXELS(0x120, 0x120),
    /* 0b00011110 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b00011111 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b00100000 */ _PIXELS(0x000, 0x000),
    /* 0b00100001 */ _PIXELS(0x000, 0x000),
    /* 0b00100010 */ _PIXELS(0x04d, 0x04d),
    /* 0b00100011 */ _PIXELS(0x04d, 0x01d),
    /* 0b00100100 */ _PIXELS(0x10f, 0x10f),
    /* 0b00100101 */ _PIXELS(0x10f, 0x1b7),
    /* 0b00100110 */ _PIXELS(0x10f, 0x10f),
    /* 0b00100111 */ _PIXELS(0x10f, 0x1b7),
    /* 0b00101000 */ _PIXELS(0x124, 0x124),
    /* 0b00101001 */ _PIXELS(0x124, 0x0fd),
    /* 0b00101010 */ _PIXELS(0x124, 0x124),
    /* 0b00101011 */ _PIXELS(0x124, 0x0fd),
    /* 0b00101100 */ _PIXELS(0x1de, 0x1de),
    /* 0b00101101 */ _PIXELS(0x1de, 0x1ff),
    /* 0b00101110 */ _PIXELS(0x1de, 0x1de),
    /* 0b00101111 */ _PIXELS(0x1de, 0x1ff),
    /* 0b00110000 */ _PIXELS(0x01d, 0x000),
    /* 0b00110001 */ _PIXELS(0x01d, 0x01d),
    /* 0b00110010 */ _PIXELS(0x01d, 0x01d),
    /* 0b00110011 */ _PIXELS(0x01d, 0x01d),
    /* 0b00110100 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b00110101 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b00110110 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b00110111 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b00111000 */ _PIXELS(0x0fd, 0x0fd),
    /* 0b00111001 */ _PIXELS(0x0fd, 0x0fd),
    /* 0b00111010 */ _PIXELS(0x0fd, 0x0fd),
    /* 0b00111011 */ _PIXELS(0x0fd, 0x0fd),
    /* 0b00111100 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b00111101 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b00111110 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b00111111 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01000000 */ _PIXELS(0x000, 0x000),
    /* 0b01000001 */ _PIXELS(0x000, 0x000),
    /* 0b01000010 */ _PIXELS(0x000, 0x000),
    /* 0b01000011 */ _PIXELS(0x000, 0x000),
    /* 0b01000100 */ _PIXELS(0x102, 0x102),
    /* 0b01000101 */ _PIXELS(0x102, 0x0db),
    /* 0b01000110 */ _PIXELS(0x10f, 0x10f),
    /* 0b01000111 */ _PIXELS(0x10f, 0x1b7),
    /* 0b01001000 */ _PIXELS(0x150, 0x150),
    /* 0b01001001 */ _PIXELS(0x150, 0x120),
    /* 0b01001010 */ _PIXELS(0x1de, 0x1de),
    /* 0b01001011 */ _PIXELS(0x1de, 0x1ff),
    /* 0b01001100 */ _PIXELS(0x150, 0x150),
    /* 0b01001101 */ _PIXELS(0x150, 0x120),
    /* 0b01001110 */ _PIXELS(0x1de, 0x1de),
    /* 0b01001111 */ _PIXELS(0x1de, 0x1ff),
    /* 0b01010000 */ _PIXELS(0x0db, 0x000),
    /* 0b01010001 */ _PIXELS(0x0db, 0x0db),
    /* 0b01010010 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b01010011 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b01010100 */ _PIXELS(0x0db, 0x0db),
    /* 0b01010101 */ _PIXELS(0x0db, 0x0db),
    /* 0b01010110 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b01010111 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b01011000 */ _PIXELS(0x120, 0x120),
    /* 0b01011001 */ _PIXELS(0x120, 0x120),
    /* 0b01011010 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01011011 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01011100 */ _PIXELS(0x120, 0x120),
    /* 0b01011101 */ _PIXELS(0x120, 0x120),
    /* 0b01011110 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01011111 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01100000 */ _PIXELS(0x000, 0x000),
    /* 0b01100001 */ _PIXELS(0x000, 0x000),
    /* 0b01100010 */ _PIXELS(0x10f, 0x10f),
    /* 0b01100011 */ _PIXELS(0x10f, 0x1b7),
    /* 0b01100100 */ _PIXELS(0x10f, 0x10f),
    /* 0b01100101 */ _PIXELS(0x10f, 0x1b7),
    /* 0b01100110 */ _PIXELS(0x10f, 0x10f),
    /* 0b01100111 */ _PIXELS(0x10f, 0x1b7),
    /* 0b01101000 */ _PIXELS(0x1de, 0x1de),
    /* 0b01101001 */ _PIXELS(0x1de, 0x1ff),
    /* 0b01101010 */ _PIXELS(0x1de, 0x1de),
    /* 0b01101011 */ _PIXELS(0x1de, 0x1ff),
    /* 0b01101100 */ _PIXELS(0x1de, 0x1de),
    /* 0b01101101 */ _PIXELS(0x1de, 0x1ff),
    /* 0b01101110 */ _PIXELS(0x1de, 0x1de),
    /* 0b01101111 */ _PIXELS(0x1de, 0x1ff),
    /* 0b01110000 */ _PIXELS(0x1b7, 0x000),
    /* 0b01110001 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b01110010 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b01110011 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b01110100 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b01110101 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b01110110 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b01110111 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b01111000 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01111001 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01111010 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01111011 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01111100 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01111101 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01111110 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b01111111 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b10000000 */ _PIXELS(0x000, 0x000),
    /* 0b10000001 */ _PIXELS(0x000, 0x000),
    /* 0b10000010 */ _PIXELS(0x000, 0x000),
    /* 0b10000011 */ _PIXELS(0x000, 0x000),
    /* 0b10000100 */ _PIXELS(0x000, 0x000),
    /* 0b10000101 */ _PIXELS(0x000, 0x000),
    /* 0b10000110 */ _PIXELS(0x000, 0x000),
    /* 0b10000111 */ _PIXELS(0x000, 0x000),
    /* 0b10001000 */ _PIXELS(0x090, 0x090),
    /* 0b10001001 */ _PIXELS(0x090, 0x060),
    /* 0b10001010 */ _PIXELS(0x124, 0x124),
    /* 0b10001011 */ _PIXELS(0x124, 0x0fd),
    /* 0b10001100 */ _PIXELS(0x150, 0x150),
    /* 0b10001101 */ _PIXELS(0x150, 0x120),
    /* 0b10001110 */ _PIXELS(0x1de, 0x1de),
    /* 0b10001111 */ _PIXELS(0x1de, 0x1ff),
    /* 0b10010000 */ _PIXELS(0x019, 0x000),
    /* 0b10010001 */ _PIXELS(0x019, 0x019),
    /* 0b10010010 */ _PIXELS(0x01d, 0x01d),
    /* 0b10010011 */ _PIXELS(0x01d, 0x01d),
    /* 0b10010100 */ _PIXELS(0x0db, 0x0db),
    /* 0b10010101 */ _PIXELS(0x0db, 0x0db),
    /* 0b10010110 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b10010111 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b10011000 */ _PIXELS(0x060, 0x060),
    /* 0b10011001 */ _PIXELS(0x060, 0x060),
    /* 0b10011010 */ _PIXELS(0x0fd, 0x0fd),
    /* 0b10011011 */ _PIXELS(0x0fd, 0x0fd),
    /* 0b10011100 */ _PIXELS(0x120, 0x120),
    /* 0b10011101 */ _PIXELS(0x120, 0x120),
    /* 0b10011110 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b10011111 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b10100000 */ _PIXELS(0x000, 0x000),
    /* 0b10100001 */ _PIXELS(0x000, 0x000),
    /* 0b10100010 */ _PIXELS(0x04d, 0x04d),
    /* 0b10100011 */ _PIXELS(0x04d, 0x01d),
    /* 0b10100100 */ _PIXELS(0x10f, 0x10f),
    /* 0b10100101 */ _PIXELS(0x10f, 0x1b7),
    /* 0b10100110 */ _PIXELS(0x10f, 0x10f),
    /* 0b10100111 */ _PIXELS(0x10f, 0x1b7),
    /* 0b10101000 */ _PIXELS(0x124, 0x124),
    /* 0b10101001 */ _PIXELS(0x124, 0x0fd),
    /* 0b10101010 */ _PIXELS(0x124, 0x124),
    /* 0b10101011 */ _PIXELS(0x124, 0x0fd),
    /* 0b10101100 */ _PIXELS(0x1de, 0x1de),
    /* 0b10101101 */ _PIXELS(0x1de, 0x1ff),
    /* 0b10101110 */ _PIXELS(0x1de, 0x1de),
    /* 0b10101111 */ _PIXELS(0x1de, 0x1ff),
    /* 0b10110000 */ _PIXELS(0x01d, 0x000),
    /* 0b10110001 */ _PIXELS(0x01d, 0x01d),
    /* 0b10110010 */ _PIXELS(0x01d, 0x01d),
    /* 0b10110011 */ _PIXELS(0x01d, 0x01d),
    /* 0b10110100 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b10110101 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b10110110 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b10110111 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b10111000 */ _PIXELS(0x0fd, 0x0fd),
    /* 0b10111001 */ _PIXELS(0x0fd, 0x0fd),
    /* 0b10111010 */ _PIXELS(0x0fd, 0x0fd),
    /* 0b10111011 */ _PIXELS(0x0fd, 0x0fd),
    /* 0b10111100 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b10111101 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b10111110 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b10111111 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11000000 */ _PIXELS(0x000, 0x000),
    /* 0b11000001 */ _PIXELS(0x000, 0x000),
    /* 0b11000010 */ _PIXELS(0x000, 0x000),
    /* 0b11000011 */ _PIXELS(0x000, 0x000),
    /* 0b11000100 */ _PIXELS(0x102, 0x102),
    /* 0b11000101 */ _PIXELS(0x102, 0x0db),
    /* 0b11000110 */ _PIXELS(0x10f, 0x10f),
    /* 0b11000111 */ _PIXELS(0x10f, 0x1b7),
    /* 0b11001000 */ _PIXELS(0x150, 0x150),
    /* 0b11001001 */ _PIXELS(0x150, 0x120),
    /* 0b11001010 */ _PIXELS(0x1de, 0x1de),
    /* 0b11001011 */ _PIXELS(0x1de, 0x1ff),
    /* 0b11001100 */ _PIXELS(0x150, 0x150),
    /* 0b11001101 */ _PIXELS(0x150, 0x120),
    /* 0b11001110 */ _PIXELS(0x1de, 0x1de),
    /* 0b11001111 */ _PIXELS(0x1de, 0x1ff),
    /* 0b11010000 */ _PIXELS(0x0db, 0x000),
    /* 0b11010001 */ _PIXELS(0x0db, 0x0db),
    /* 0b11010010 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b11010011 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b11010100 */ _PIXELS(0x0db, 0x0db),
    /* 0b11010101 */ _PIXELS(0x0db, 0x0db),
    /* 0b11010110 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b11010111 */ _PIXELS(0x1b7, 0x1b7),
    /* 0b11011000 */ _PIXELS(0x120, 0x120),
    /* 0b11011001 */ _PIXELS(0x120, 0x120),
    /* 0b11011010 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11011011 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11011100 */ _PIXELS(0x120, 0x120),
    /* 0b11011101 */ _PIXELS(0x120, 0x120),
    /* 0b11011110 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11011111 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11100000 */ _PIXELS(0x000, 0x000),
    /* 0b11100001 */ _PIXELS(0x000, 0x000),
    /* 0b11100010 */ _PIXELS(0x10f, 0x10f),
    /* 0b11100011 */ _PIXELS(0x10f, 0x1b7),
    /* 0b11100100 */ _PIXELS(0x10f, 0x10f),
    /* 0b11100101 */ _PIXELS(0x10f, 0x1b7),
    /* 0b11100110 */ _PIXELS(0x10f, 0x10f),
    /* 0b11100111 */ _PIXELS(0x10f, 0x1b7),
    /* 0b11101000 */ _PIXELS(0x1de, 0x1de),
    /* 0b11101001 */ _PIXELS(0x1de, 0x1ff),
    /* 0b11101010 */ _PIXELS(0x1de, 0x1de),
    /* 0b11101011 */ _PIXELS(0x1de, 0x1ff),
    /* 0b11101100 */ _PIXELS(0x1de, 0x1de),
    /* 0b11101101 */ _PIXELS(0x1de, 0x1ff),
    /* 0b11101110 */ _PIXELS(0x1de, 0x1de),
    /* 0b11101111 */ _PIXELS(0x1de, 0x1ff),
    /* 0b11110000 */ _PIXELS(0x1ff, 0x000),
    /* 0b11110001 */ _PIXELS(0x1ff, 0x1b7),
    /* 0b11110010 */ _PIXELS(0x1ff, 0x1b7),
    /* 0b11110011 */ _PIXELS(0x1ff, 0x1b7),
    /* 0b11110100 */ _PIXELS(0x1ff, 0x1b7),
    /* 0b11110101 */ _PIXELS(0x1ff, 0x1b7),
    /* 0b11110110 */ _PIXELS(0x1ff, 0x1b7),
    /* 0b11110111 */ _PIXELS(0x1ff, 0x1b7),
    /* 0b11111000 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11111001 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11111010 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11111011 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11111100 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11111101 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11111110 */ _PIXELS(0x1ff, 0x1ff),
    /* 0b11111111 */ _PIXELS(0x1ff, 0x1ff),
};
#undef _PIXELS
