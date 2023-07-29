# Apple II VGA Card

This project is a VGA card for Apple II computers to ouput a crisp RGB signal to a
VGA monitor instead of having to rely on the composite output. This is accomplished
by snooping the 6502 bus and creating a shadow copy of the video memory within a
Raspberry Pi Pico, then processing the raw video memory contents to output a "perfect"
signal.

    +------+                +-----+      +-----------+
    | 6502 |________________| RAM |______| Composite | 
    | CPU  |  System   \    +-----+      | Video Gen |
    +------+   Bus     |                 +-----------+
                       |    +-----------+
                       \____| Raspberry |_________________\
                            |  Pi Pico  |   640x480 VGA   /
                            +-----------+

This project is currently in a pretty raw state so consider it useful for informational
purposes only. I've built one prototype card and several other people have also been able to
build a working card so it has been tested on a handful of Apple II+/IIe's and VGA monitors
but YMMV.

These features are currently supported:
 * Generates a 640x480@60 VGA signal with 3 bits per color channel using resistor DACs
 * Text mode (monochrome)
 * Lo-res mode with no color fringing between the chunky pixels
 * Double Lo-res mode (Apple IIe only)
 * Hi-res mode with simulated NTSC artifact color
 * Double Hi-res mode (Apple IIe only)
 * Mixed lo-res and hi-res modes with monochrome text and no color fringing
 * 80 Columms text support (Apple IIe only)
 * Soft-monochrome mode to force hires display as if on a monochrome monitor
 * Some Video-7 RGB card extended graphical modes

I had these goals in mind during design:
 * Generate video out to a more modern display - I don't have any old CRTs for
   displaying composite signals any more and composite to HDMI adapters don't work well.
 * Generate crisp video - I wanted "perfect" video output, the way that it should have
   been barring technical difficulties of displays in 1980. So like, no NTSC artifact
   coloring where it's not supposed to be.
 * Non-invasive - I didn't want to have to solder or modify my logic board.
 * Common parts - The parts to build should be cheap and easy to get anywhere,
   just like the original Apple II
 * Open-source - If this helps anyone else make their Apple II better then that's
   a bonus.

I also wanted to see if a Pi Pico could actually work on an 8-bit CPU bus, since the docs
say it should work but there were no code examples.

Included in the repo here are the schematics (KiCAD project, [schematics PDF](AppleVGA/AppleVGA.pdf) and [Bill-Of-Materials CSV](AppleVGA/AppleVGA_BOM.csv)) and the [source code for the Pi Pico
microcontroller](pico/). I used the unrouted PCB layout to wire up the initial prototype
board on a [Glitchworks Apple II Prototyping Board](https://www.tindie.com/products/glitchwrks/gw-a2-1-glitchworks-apple-ii-prototyping-board/).

![Prototype Card](docs/prototype_card.jpg)


## Comparisons

Here are a few comparisons of the VGA card output vs composite video through
a cheap composite -> HDMI adapter

**Text**
![Text Mode](docs/composite_vs_vga_text.jpg)

**Lores**
![Lores Mode](docs/composite_vs_vga_lores.jpg)

**Hires**
![Hires Mode](docs/composite_vs_vga_hires.jpg)

**DHires**
![DHires Mode](docs/composite_vs_vga_dhires.jpg)

**80 Columms**
![80 Columms Mode](docs/composite_vs_vga_80columms.jpg)


## Future work

There are a few things that I think are easily possible with this design and I might
try to implement:
 * loading custom character ROMs from Basic
 * Palettes for hires mode (color/mono-white/mono-green)


## Limitations

This VGA card design currently does have some limitations:
 * The VGA signal vsync is not synchronized with the Apple II video memory scanning so
   software that tries to detect the composite video vsync using 'vapor lock' may not
   look good.
 * All the Pi Pico GPIOs are being used so connecting to slot 7's SYNC or any other bus
   signal is a bit of a challenge without adding more chips or optimizing the DACs.
