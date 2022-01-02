#!/usr/bin/env python3


import sys
from math import cos, sin, radians, pi, isclose

import numpy as np


# standard
YIQ_TO_RGB = np.array([
    [ 1.0, 0.956, 0.619 ],
    [ 1.0, -0.272, -0.647 ],
    [ 1.0, -1.106, 1.703 ]
]).transpose()

# for NTSC
NTSC_YIQ_TO_RGB = np.array([
    [ 1.0, 0.9469, 0.6263 ],
    [ 1.0, -0.2748, -0.6357 ],
    [ 1.0, -1.1, 1.7 ],
]).transpose()

PALETTE_OLD = [
    (0,0,0),          # 0000 black
    (0xce,0xd4,0x13), # 0001 d.brown
    (0x00,0x73,0x10), # 0010 d.bluegreen
    (0x12,0xca,0x07), # 0011 h.green
    (0x42,0x04,0xe1), # 0100 d.blue
    (0x8f,0x8f,0x8f), # 0101 l.gray
    (0x24,0x97,0xff), # 0110 h.blue
    (0x51,0xf5,0x95), # 0111 l.blgreen
    (0x99,0x03,0x5f), # 1000 d.magenta
    (0xf0,0x5c,0x00), # 1001 h.orange
    (0x6f,0x6f,0x6f), # 1010 d.gray
    (0xce,0xd4,0x13), # 1011 l.brown
    (0xca,0x13,0xfe), # 1100 h.violet
    (0xff,0x85,0xe1), # 1101 l.magenta
    (0xaa,0xa2,0xff), # 1110 l.blue
    (0xff,0xff,0xff), # 1111 white
]

PALETTE = [
    (0,0,0),        # 0000 black
    (72,74,0),      # 0001 d.brown
    (0,98,30),      # 0010 d.bluegreen
    (29,143,0),     # 0011 h.green
    (41,40,183),    # 0100 d.blue
    (142,142,142),  # 0101 l.gray
    (0,110,185),    # 0110 h.blue
    (127,240,171),  # 0111 l.blgreen
    (128,15,84),    # 1000 d.magenta
    (171,60,0),     # 1001 h.orange
    (113,113,113),  # 1010 d.gray
    (157,159,15),   # 1011 l.brown
    (141,27,239),   # 1100 h.violet
    (255,127,225),  # 1101 l.magenta
    (211,210,255),  # 1110 l.blue
    (255,255,255),  # 1111 white
]

def area(a_min, a_max):
    a_min = a_min % 360
    if a_min > 180:
        a_min -= 360
    if a_min < -90:
        a_min = -90

    a_max = a_max % 360
    if a_max > 180:
        a_max -= 360
    if a_max > 90:
        a_max = 90

    # sum( cos(x) dx ) for dx = a_min .. a_max
    #            \/
    return max(0.0, sin(radians(a_max)) - sin(radians(a_min)))


def sinc(x):
    return 1.0 if isclose(x, 0.0) else sin(x) / x


def clamp(n, min_value, max_value):
    return max(min(n, max_value), min_value)


def round_int(arr):
    return np.floor(arr + 0.5)


def normalize(rgb):
    div = rgb.max()
    if div > 1.0:
        rgb /= div
    return rgb.clip(0.0, 1.0)


def phase_shift(bits, phase):
    if phase == 0:
        return bits
    elif phase == 90:
        return (bits >> 1) | ((bits & 0x01) << 3)
    elif phase == 180:
        return (bits >> 2) | ((bits & 0x03) << 2)
    elif phase == 270:
        return (bits >> 3) | ((bits & 0x07) << 1)
    else:
        assert False, "invalid phase"


# bits - 7 bits, where bit 3 is the one being rendered
def color_of(phase_ref, bits):
    #phase_ref = (phase_ref + 270) % 360
    rgb = (255, 0, 0) # XXX

    if (bits >> 3) & 0xf == 0 or (bits >> 2) & 0xf == 0 or (bits >> 1) & 0xf == 0 or bits & 0xf == 0:
        rgb = PALETTE[0]
    elif (bits >> 3) & 0xf == 0xf or (bits >> 2) & 0xf == 0xf or (bits >> 1) & 0xf == 0xf or bits & 0xf == 0xf:
        rgb = PALETTE[15]
    else:
        rgb = PALETTE[phase_shift(bits, phase_ref) & 0xf]

    return np.array(rgb) / 255


def rgb8_color_of(*args):
    return round_int(color_of(*args) * 255)


def print_stuff():
    print("")
    print("[1000] d.magenta = {} ({})".format(rgb8_color_of(0.0, 0b0001000), (0x99,0x03,0x5f)))
    print("[0100] d.blue = {} ({})".format(rgb8_color_of(90.0, 0b0001000), (0x42,0x04,0xe1)))
    print("[0010] d.bluegreen = {} ({})".format(rgb8_color_of(180.0, 0b0001000), (0x00,0x73,0x10)))
    print("[0001] d.brown = {} ({})".format(rgb8_color_of(270.0, 0b0001000), (0xce,0xd4,0x13)))
    print("")
    print("[0001111] white = {} ({})".format(rgb8_color_of(0.0, 0b0001111), (0xff,0xff,0xff)))
    print("[0011110] white = {} ({})".format(rgb8_color_of(90.0, 0b0011110), (0xff,0xff,0xff)))
    print("[0111100] white = {} ({})".format(rgb8_color_of(180.0, 0b0111100), (0xff,0xff,0xff)))
    print("")
    print("[1100] h.violet = {} ({})".format(rgb8_color_of(0.0, 0b0001100), (0xca,0x13,0xfe)))
    print("[0110] h.blue = {} ({})".format(rgb8_color_of(90.0, 0b0001100), (0x24,0x97,0xff)))
    print("[0011] h.green = {} ({})".format(rgb8_color_of(180.0, 0b0001100), (0x12,0xca,0x07)))
    print("[1001] h.orange = {} ({})".format(rgb8_color_of(270.0, 0b0001100), (0xf0,0x5c,0x00)))
    print("")
    print("[1010] d.gray = {} ({})".format(rgb8_color_of(180.0, 0b0101000), (0x7f,0x7f,0x7f)))
    print("[0101] l.gray = {} ({})".format(rgb8_color_of(180.0, 0b0010100), (0x7f,0x7f,0x7f)))
    print("")
    print("[1110] l.blue = {} ({})".format(rgb8_color_of(180.0, 0b0111000), (0xaa,0xa2,0xff)))
    print("[0111] l.blgreen = {} ({})".format(rgb8_color_of(180.0, 0b0011100), (0x51,0xf5,0x95)))
    print("[1011] l.brown = {} ({})".format(rgb8_color_of(180.0, 0b0101100), (0xce,0xd4,0x13)))
    print("[1101] l.magenta = {} ({})".format(rgb8_color_of(180.0, 0b0110100), (0xff,0x85,0xe1)))
    sys.exit(0)
#print_stuff()

def print_stuff_2():
    #           |
    print("[00000011] {}".format(rgb8_color_of(180.0, 0, 0, 0, 0)))
    print("[00000111] {}".format(rgb8_color_of(270.0, 0, 0, 0, 1)))
    print("[00001110] {}".format(rgb8_color_of(0.0, 0, 0, 1, 1)))
    print("[00011100] {}".format(rgb8_color_of(90.0, 0, 1, 1, 1)))
    print("[00111000] {}".format(rgb8_color_of(180.0, 1, 1, 1, 0))) # OK
    print("[01110000] {}".format(rgb8_color_of(270.0, 1, 1, 0, 0)))
    print("[11100000] {}".format(rgb8_color_of(0.0, 1, 0, 0, 0)))
    print("[11000000] {}".format(rgb8_color_of(90.0, 0, 0, 0, 0)))
    sys.exit(0)
#print_stuff_2()


fp = sys.stdout

fp.write("#pragma once\n\n")
fp.write("#define _PIXELS(color1, color2) ((uint32_t)(color1) | ((uint32_t)(color2) << 16))\n")
fp.write("static uint32_t hires_color_patterns[2*256] = {\n")
fp.write("    // Even byte pixels\n")
for bits in range(256):
    col1 = round_int(color_of(0.0, bits >> 1) * 7)
    col2 = round_int(color_of(90.0, bits & 0x7f) * 7)

    col1_packed = (int(col1[0]) << 6) | (int(col1[1]) << 3) | int(col1[2])
    col2_packed = (int(col2[0]) << 6) | (int(col2[1]) << 3) | int(col2[2])
    fp.write("    /* 0b{:08b} */ _PIXELS(0x{:03x}, 0x{:03x}),\n".format(bits, col1_packed, col2_packed))

fp.write("\n    // Odd byte pixels\n")
for bits in range(256):
    col1 = round_int(color_of(180.0, bits >> 1) * 7)
    col2 = round_int(color_of(270.0, bits & 0x7f) * 7)

    col1_packed = (int(col1[0]) << 6) | (int(col1[1]) << 3) | int(col1[2])
    col2_packed = (int(col2[0]) << 6) | (int(col2[1]) << 3) | int(col2[2])
    fp.write("    /* 0b{:08b} */ _PIXELS(0x{:03x}, 0x{:03x}),\n".format(bits, col1_packed, col2_packed))

fp.write("};\n")
fp.write("#undef _PIXELS\n")
