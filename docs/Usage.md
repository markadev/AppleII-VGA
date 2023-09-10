# Using the VGA card

After you load on the firmware for your respective Apple II system (II+ or IIe)
the VGA card is generally plug-and-play. Plug it in, power on your Apple II
and that's it.


## Soft Monochrome Mode

Due to the way hires mode works to generate color, some software looks best on a monochrome
monitor where close pixels won't blend together into a colored mess. A software-controlled
monochrome mode can be enabled to disable the hires mode artifact color simulation.

This can be changed by writing to bit 7 of soft-switch at address `$c021`. From Applesoft Basic:

    REM ENABLE SOFT MONOCHROME
    POKE 49185, 128

    REM DISABLE SOFT MONOCHROME
    POKE 49185, 0

When enabled this will:
 * Change all modes to render as if it was connected to a high resolution monochrome monitor.
   "Colors" originally created by NTSC artifacting will appear as sequences of dots.
 * Switch to the monochrome color palette (currently only black & white)


## Device Registers

The card's device registers can be written to change some settings. *These settings are
not saved between power-cycles*

The register base address depends on the slot number in which you installed the card:

    $c080 + $10*n

Or in Applesoft Basic:

    SLOT=4
    BASEADDR=49280+16*SLOT

The various register offsets can then be written to:

    POKE BASEADDR, <value>
    POKE BASEADDR+1, <value>
    POKE BASEADDR+2, <value>
    ...


### Register 0 - Feature Select

**[write-only]**
This register controls enablement of some features of the card

| bit(s) | Description
| ------ | -----------
|  7:2   | reserved
|   1    | Setting to 1 will disable simulated scanline rendering
|   0    | Setting to 1 will enable simulated scanline rendering


### Register 1 - Monochrome color selection

**[write-only]**
This register controls the output color when soft-monochrome mode is enabled

| bit(s) | Description
| ------ | -----------
|  7:6   | reserved
|  5:4   | Setting non-zero will choose a background color (black, green, amber)
|  3:2   | reserved
|  1:0   | Setting non-zero will choose a foreground color (black, green, amber)
