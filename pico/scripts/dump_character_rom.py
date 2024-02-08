#!/usr/bin/env python3

import argparse
import sys


def parse_args():
    parser = argparse.ArgumentParser(description='Reads an Apple II video character ROM and transforms it to the internal format used by the AppleII-VGA firmware')
    parser.add_argument('-t', choices=['ii+', 'iij+', 'iie', 'videx'], required=True, help='Machine type of the character ROM')
    parser.add_argument('filename', help='Name of the ROM file to dump')
    return parser.parse_args()


def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)


def main():
    args = parse_args()
    with open(args.filename, 'rb') as fp:
        char_bytes = fp.read(2048)
        if len(char_bytes) < 2048:
            eprint("ROM file is expected to be 2KiB")
            sys.exit(1)

    if args.t == 'ii+':
        charset_offsets = {
            0x00: 'Inverse alphabet',
            0x20: 'Inverse symbols',
            0x40: 'Flashing alphabet',
            0x60: 'Flashing symbols',
            0x80: 'Normal control',
            0xa0: 'Normal symbols',
            0xc0: 'Upper alpha',
            0xe0: 'Lower alpha',
        }
    elif args.t == 'iie':
        charset_offsets = {
            0x00: 'Inverse control (upper)',
            0x20: 'Inverse special',
            0x40: 'Mousetext',
            0x60: 'Inverse lower',
            0x80: 'Normal control (upper)',
            0xa0: 'Normal special',
            0xc0: 'Normal upper',
            0xe0: 'Normal lower',
        }
    elif args.t == 'iij+':
        charset_offsets = {
            0x00: 'Normal katakana',
            0x40: 'Flashing katakana',
            0x80: 'Normal control',
            0xa0: 'Normal symbols',
            0xc0: 'Upper alpha',
            0xe0: 'Lower alpha',
        }

    elif args.t == 'videx':
        charset_offsets = {
            0x00: '',
            0x40: '',
            0x80: '',
            0xa0: '',
            0xc0: '',
            0xe0: '',
        }

    if args.t == 'videx':
        charset_name = ""
        print("const uint8_t default_character_rom[128 * 16] = {")
        for ch in range(128):
            if ch in charset_offsets:
                # Print a nice header
                charset_name = charset_offsets[ch]
                if ch > 0:
                    print("")
                print("    //")
                print(f"    // {charset_name} Set")
                print("    //")

            print(f"\n    // {charset_name} character 0x{ch:02x}")
            for i in range(16):
                char_byte = char_bytes[16*ch + i]
                char_bits = f"{char_byte:08b}"
                char_bits = char_bits[::-1]
                print(f"    0b{char_bits},")
        print("};")
    else:

        charset_name = ""
        print("const uint8_t default_character_rom[256 * 8] = {")
        for ch in range(256):
            if ch in charset_offsets:
                # Print a nice header
                charset_name = charset_offsets[ch]
                if ch > 0:
                    print("")
                print("    //")
                print(f"    // {charset_name} Set")
                print("    //")

            print(f"\n    // {charset_name} character 0x{ch:02x}")

            for i in range(8):
                char_byte = char_bytes[8*ch + i]
                # Convert the character pattern into the normalized format expected by the AppleII-VGA firmware
                #   * bit[7] on Apple II+ is used to indicate a flashing character in the lower half of the character set
                #   * bit[6:0] are the pattern where 1 represents an on pixel and will be rendered from LSB to MSB
                #     (which will look mirrored if you examine the resulting bit pattern directly).
                if args.t == 'iie':
                    # IIe ROM patterns are stored with 0 representing an on pixel, so invert
                    char_byte ^= 0x7f

                    # bit 7 is not used by IIe character sets so just clear it
                    char_byte &= 0x7f
                elif args.t == 'ii+':
                    # II+ inverse character ROM patterns are stored with 0 representing an on pixel and bit7==0 to indicate
                    # that the hardware should invert the pattern. Just invert those patterns here.
                    if ch < 0x80 and (char_byte & 0x80) == 0:
                        char_byte ^= 0x7f

                    # bit 7 is not used in the upper half of II+ characters sets so just clear it
                    if ch >= 0x80:
                        char_byte &= 0x7f
                elif args.t == 'iij+':
                    # Translate with katakana always enabled, so map 0x40-0x7f to 0x00-0x3f
                    if ch >= 0x40 and ch <= 0x7f:
                        char_byte = char_bytes[8*(ch-0x40) + i]

                    # 0x00 to 0x3f are supposed to be normal characters so make the bits right
                    if ch <= 0x3f:
                        char_byte ^= 0x7f
                        char_byte &= 0x7f

                    # II+ inverse character ROM patterns are stored with 0 representing an on pixel and bit7==0 to indicate
                    # that the hardware should invert the pattern. Just invert those patterns here.
                    #if ch < 0x80 and (char_byte & 0x80) == 0:
                    #    char_byte ^= 0x7f

                    # bit 7 is not used in the upper half of II+ characters sets so just clear it
                    if ch >= 0x80:
                        char_byte &= 0x7f

                char_bits = f"{char_byte:08b}"
                if args.t in ('ii+', 'iij+'):
                    # II+ ROMs store the character pattern from left to right in bit6 to bit0 (the most significant
                    # bit is drawn to screen first). The AppleII-VGA firmware expect the pattern to be in the opposite
                    # bit order so reverse those bits here.
                    char_bits = char_bits[0] + char_bits[:0:-1]

                print(f"    0b{char_bits},")
        print("};")


if __name__ == '__main__':
    main()
