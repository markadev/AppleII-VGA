#!/usr/bin/env python3

import numpy as np
import pygame

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


TEST_PATTERN = [
    np.array(20*[1, 1, 0, 0] + 4*[0]),
    np.array(20*[0, 1, 1, 0] + 4*[0]),
    np.array(20*[0, 0, 1, 1] + 4*[0]),
    np.array([0, 0, 0, 1] + 19*[1, 0, 0, 1] + [1, 0, 0, 0]),
    np.array([0, 0, 1, 1,  0, 0, 0, 0, 0,  1, 1,  0, 0, 0, 0, 0,  1, 1,  0, 0, 0, 0, 0,  1, 1,  0, 0, 0, 0, 0 ]),
    np.array(8*[0, 0, 0, 0, 1, 1, 1, 1] + 4*[0]),
    np.array(4*[0] + 3*[1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0]),
    np.array(3*[0] + 3*[1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0]),
    np.array(2*[0] + 3*[1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0]),
    np.array([0] + 3*[1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0]),
    np.zeros(10),
    np.repeat([0,1,0,0,0,1,0, 0,1,1,1,1,1,0, 0,1,0,0,0,0,0, 0,1,0,0,0,0,0, 0,0,1,1,1,0,0, 0,0], 2),
    np.repeat([0,1,0,0,0,1,0, 0,1,0,0,0,0,0, 0,1,0,0,0,0,0, 0,1,0,0,0,0,0, 0,1,0,0,0,1,0, 0,0], 2),
    np.repeat([0,1,0,0,0,1,0, 0,1,0,0,0,0,0, 0,1,0,0,0,0,0, 0,1,0,0,0,0,0, 0,1,0,0,0,1,0, 0,0], 2),
    np.repeat([0,1,1,1,1,1,0, 0,1,1,1,1,0,0, 0,1,0,0,0,0,0, 0,1,0,0,0,0,0, 0,1,0,0,0,1,0, 0,0], 2),
    np.repeat([0,1,0,0,0,1,0, 0,1,0,0,0,0,0, 0,1,0,0,0,0,0, 0,1,0,0,0,0,0, 0,1,0,0,0,1,0, 0,0], 2),
    np.repeat([0,1,0,0,0,1,0, 0,1,0,0,0,0,0, 0,1,0,0,0,0,0, 0,1,0,0,0,0,0, 0,1,0,0,0,1,0, 0,0], 2),
    np.repeat([0,1,0,0,0,1,0, 0,1,1,1,1,1,0, 0,1,1,1,1,1,0, 0,1,1,1,1,1,0, 0,0,1,1,1,0,0, 0,0], 2),
    np.zeros(10),
    np.array(20*[1, 0, 0, 0] + 4*[0]),
    np.array(20*[0, 1, 0, 0] + 4*[0]),
    np.array(20*[1, 1, 0, 0] + 4*[0]),
    np.array(20*[0, 0, 1, 0] + 4*[0]),
    np.array(20*[1, 0, 1, 0] + 4*[0]),
    np.array(20*[0, 1, 1, 0] + 4*[0]),
    np.array(20*[1, 1, 1, 0] + 4*[0]),
    np.array(20*[0, 0, 0, 1] + 4*[0]),
    np.array(20*[1, 0, 0, 1] + 4*[0]),
    np.array(20*[0, 1, 0, 1] + 4*[0]),
    np.array(20*[1, 1, 0, 1] + 4*[0]),
    np.array(20*[0, 0, 1, 1] + 4*[0]),
    np.array(20*[1, 0, 1, 1] + 4*[0]),
    np.array(20*[0, 1, 1, 1] + 4*[0]),
    np.array(20*[1, 1, 1, 1] + 4*[0]),
]


# dots - An array of at least 7 samples, where the 4th is the dot being rendered and previous samples are previously rendered dots
#  0  1  2  3  4  5  6  ....
#  \_____/  ^  \_____/
#   past    |  future
#   dots    |   dots
#        current
#          dot
def color_of(color_phase_deg, dots):
    assert dots.shape[0] >= 7
    yiq = decoder.decode_one(dots[0:7], color_phase_deg)
    return palette.yiq_to_rgb(yiq)


def game_loop():
    pygame.init()

    screen = pygame.display.set_mode((1280, 720))
    screen.fill(pygame.Color(0, 0, 0))

    clock = pygame.time.Clock()
    running = True
    redraw = True
    y = 0

    pix_width = 2
    pix_height = 4 * 4

    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    running = False
                elif event.key in (pygame.K_t, pygame.K_g):
                    delta = 0.05 if event.key == pygame.K_t else -0.05
                    decoder.color_saturation += delta
                    print(f"COLOR_SATURATION={decoder.color_saturation}")
                elif event.key in (pygame.K_y, pygame.K_h):
                    delta = 0.05 if event.key == pygame.K_y else -0.05
                    decoder.contrast += delta
                    print(f"CONTRAST={decoder.contrast}")
                elif event.key == pygame.K_EQUALS:
                    pix_width *= 2
                elif event.key == pygame.K_MINUS:
                    if pix_width > 1:
                        pix_width //= 2
                elif event.key == pygame.K_SPACE:
                    decoder.smooth = not decoder.smooth
                elif event.key == pygame.K_KP7:
                    YIQ_WEIGHTS[0] += 0.05
                    print(f"YIQ_WEIGHTS={YIQ_WEIGHTS}")
                elif event.key == pygame.K_KP4:
                    YIQ_WEIGHTS[0] -= 0.05
                    print(f"YIQ_WEIGHTS={YIQ_WEIGHTS}")
                elif event.key == pygame.K_KP8:
                    YIQ_WEIGHTS[1] += 0.05
                    print(f"YIQ_WEIGHTS={YIQ_WEIGHTS}")
                elif event.key == pygame.K_KP5:
                    YIQ_WEIGHTS[1] -= 0.05
                    print(f"YIQ_WEIGHTS={YIQ_WEIGHTS}")
                elif event.key == pygame.K_KP9:
                    YIQ_WEIGHTS[2] += 0.05
                    print(f"YIQ_WEIGHTS={YIQ_WEIGHTS}")
                elif event.key == pygame.K_KP6:
                    YIQ_WEIGHTS[2] -= 0.05
                    print(f"YIQ_WEIGHTS={YIQ_WEIGHTS}")
                redraw = True

        if not running:
            break

        if redraw:
            y = 0
            redraw = False

        for i in range(2):
            signal = np.append(np.zeros(3), TEST_PATTERN[y])

            line_rect = pygame.Rect(0, y*pix_height, screen.get_width(), pix_height)
            pygame.draw.rect(screen, pygame.Color(0, 0, 0), line_rect)

            phase = 0
            for x in range(min(len(signal) - 6, screen.get_width() // pix_width)):
                rgb = scale_to_int(color_of(phase, signal[x:]), 256)

                phase = (phase + 90) % 360
                c = pygame.Color(int(rgb[0]), int(rgb[1]), int(rgb[2]))
                pygame.draw.rect(screen, c, pygame.Rect(pix_width*x, pix_height*y, pix_width, pix_height))

            pygame.display.update(line_rect)
            y = (y + 1) % len(TEST_PATTERN)

        clock.tick(60)  # limits FPS to 60

    pygame.quit()


if __name__ == '__main__':
    game_loop()
