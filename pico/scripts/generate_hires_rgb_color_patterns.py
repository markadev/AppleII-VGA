#!/usr/bin/env python3

import sys

import numpy as np
from color import *


PRIMARY_COLORS = [
    (0x00,0x00,0x00),   # 0000 black
    (0x93,0x0b,0x7c),   # 0001 magenta
    (0x1f,0x35,0xd3),   # 0010 d.blue
    (0xcc,0x36,0xee),   # 0011 h.violet
    (0x00,0x76,0x0c),   # 0100 d.green
    (0x88,0x88,0x88),   # 0101 gray1
    (0x00,0xbe,0xf0),   # 0110 h.blue
    (0x9d,0xac,0xff),   # 0111 l.blue
    (0x62,0x4c,0x00),   # 1000 brown
    (0xff,0x90,0x10),   # 1001 h.orange
    (0x88,0x88,0x88),   # 1010 gray2
    (0xff,0x81,0xec),   # 1011 pink
    (0x43,0xc8,0x00),   # 1100 h.green
    (0xee,0xee,0x33),   # 1101 yellow
    (0x55,0xee,0xaa),   # 1110 aqua
    (0xff,0xff,0xff),   # 1111 white
]
decoder = DiscreteColorDecoder(PRIMARY_COLORS)
YIQ_WEIGHTS = [ 1.0, 1.0, 1.0 ]
palette = QuantizedYIQPalette(weights=YIQ_WEIGHTS)


# dots - A bitmap of 7 dots, where bit 3 is the one being rendered and lower bits are previously scanned dots
#  6  5  4  3  2  1  0
#  \_____/  ^  \_____/
#  future   |   past
#   dots    |   dots
#        current
#          dot
def color_of(color_phase_deg, dots):
    d = np.array([
        dots & 1,
        (dots >> 1) & 1,
        (dots >> 2) & 1,
        (dots >> 3) & 1,
        (dots >> 4) & 1,
        (dots >> 5) & 1,
        (dots >> 6) & 1,
    ])
    yiq = decoder.decode_one(d, color_phase_deg)
    return palette.yiq_to_rgb(yiq)


fp = sys.stdout

fp.write("#pragma once\n\n")
fp.write("// clang-format off\n")
fp.write("#define _PIXELS(color1, color2) ((uint32_t)(color1) | ((uint32_t)(color2) << 16))\n")
fp.write("static uint32_t hires_rgb_color_patterns[2 * 256] = {\n")
fp.write("    // Even byte pixels\n")
for bits in range(256):
    col1 = scale_to_int(color_of(0, bits), 8)
    col2 = scale_to_int(color_of(90, bits >> 1), 8)

    col1_packed = (int(col1[0]) << 6) | (int(col1[1]) << 3) | int(col1[2])
    col2_packed = (int(col2[0]) << 6) | (int(col2[1]) << 3) | int(col2[2])
    fp.write("    /* 0b{:08b} */ _PIXELS(0x{:03x}, 0x{:03x}),\n".format(bits, col1_packed, col2_packed))

fp.write("\n    // Odd byte pixels\n")
for bits in range(256):
    col1 = scale_to_int(color_of(180, bits), 8)
    col2 = scale_to_int(color_of(270, bits >> 1), 8)

    col1_packed = (int(col1[0]) << 6) | (int(col1[1]) << 3) | int(col1[2])
    col2_packed = (int(col2[0]) << 6) | (int(col2[1]) << 3) | int(col2[2])
    fp.write("    /* 0b{:08b} */ _PIXELS(0x{:03x}, 0x{:03x}),\n".format(bits, col1_packed, col2_packed))

fp.write("};\n")
fp.write("#undef _PIXELS\n")
