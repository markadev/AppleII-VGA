#!/usr/bin/env python3

import sys

fp = sys.stdout

fp.write("#pragma once\n\n")
fp.write("// clang-format off\n")
fp.write("#include <stdint.h>\n\n")

fp.write("// Mapping of a hires video byte to 14 half-pixel dots.\n")
fp.write("// Bits are displayed from LSB to MSB.\n")
fp.write("static uint16_t hires_dot_patterns[256] = {")
for hires_byte in range(256):
    if hires_byte % 8 == 0:
        fp.write("\n    ")

    # hires byte consists of
    # bit 7 - delay by 1/2 'pixel'
    # bits 6:0 - pixel bits, displayed from LSB to MSB

    # Want to output a doubled resolution bitmap of 14 dots
    # that is shifted according to the MSB
    dots = hires_byte & 0x7f
    dots = (dots | (dots << 4)) & 0x0f0f
    dots = (dots | (dots << 2)) & 0x3333
    dots = (dots | (dots << 1)) & 0x5555
    dots |= (dots << 1)

    if hires_byte & 0x80:
        # Delay output by one dot (half a pixel)
        dots = (dots << 1) & 0x3fff

    #print("0b{:014b}".format(dots))
    fp.write("0x{:04x},".format(dots))

fp.write("\n};\n")
