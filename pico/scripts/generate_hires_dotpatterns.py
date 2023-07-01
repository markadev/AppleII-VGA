#!/usr/bin/env python3

import sys

fp = sys.stdout

fp.write("#pragma once\n\n")
fp.write("// clang-format off\n")
fp.write("#include <stdint.h>\n\n")

fp.write("// Mapping of a hires video byte to 14 half-pixel dots.\n")
fp.write("// Bits are displayed from MSB to LSB.\n")
fp.write("static uint16_t hires_dot_patterns[256] = {")
for hiresByteValue in range(256):
    if hiresByteValue % 8 == 0:
        fp.write("\n    ")
    bits = 0

    # hires byte consistes of
    # MSB - delay by 1/2 'pixel'
    # bits[0:6] - pixel bits, displayed from LSB to MSB

    # Want to output a doubled resolution bitmap of 14 bits
    # that is shifted according to the MSB, and bit-reversed so
    # it is displayed MSB to LSB

    for i in range(7):
        bits <<= 2
        if hiresByteValue & 1:
            bits |= 0x3
        hiresByteValue >>= 1
    if hiresByteValue & 1:
        # Output pixels are delayed by an extra half-pixel
        bits >>= 1
    #print("0b{:014b}".format(bits))
    fp.write("0x{:04x},".format(bits))

fp.write("\n};\n")
