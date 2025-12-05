#!/usr/bin/env python3

import numpy as np
import sys

from color import *


decoder = SimpleAnalogColorDecoder()
YIQ_WEIGHTS = [ 1.35, 1.0, 1.0 ]
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
fp.write("static uint32_t hires_ntsc_color_patterns[2 * 256] = {\n")
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
