# Using the VGA card

After you load on the firmware for your respective Apple II system (II+ or IIe)
the VGA card is generally plug-and-play. Plug it in, power on your Apple II
and that's it.

There are some optional features and other controls that can be changed using the
menu-based Configuration disk or by directly using `POKE` commands.


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
 * Switch to the monochrome color palette (black & white by default)

This setting will be saved across power-cycles if the "Save" command is executed.


## Device Registers

The card's device registers can be written to change some settings. These settings persist across
CPU resets and will be saved permanently if the "Save" command is executed (see register 4).

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
|  7:4   | reserved
|   3    | Setting to 1 will disable Videx VideoTerm support (II+ only)
|   2    | Setting to 1 will enable Videx VideoTerm support (II+ only)
|   1    | Setting to 1 will disable simulated scanline rendering
|   0    | Setting to 1 will enable simulated scanline rendering


### Register 1 - Monochrome color selection

**[write-only]**
This register controls the output color when soft-monochrome mode is enabled

| bit(s) | Description
| ------ | -----------
|  7:6   | reserved
|  5:4   | Setting non-zero will choose a background color (1=black, 2=green, 3=amber)
|  3:2   | reserved
|  1:0   | Setting non-zero will choose a foreground color (1=white, 2=green, 3=amber)


### Registers 2 & 3 - Text character pattern offset & data

**[write-only]**
These registers allow the character patterns used by the text mode character generator
to be modified. To update the patterns of one or more chacters you would first write the
character number of the character to update to register 2, and then write 8 bytes of the
character's bit pattern to register 3. To update multiple contiguous characters you can
just continue writing 8 bytes per character to update.

For example, to replace the '#' character (character number `$A3`) with '£':

    POKE BASEADDR+2, 163
    POKE BASEADDR+3, 184
    POKE BASEADDR+3, 196
    POKE BASEADDR+3, 132
    POKE BASEADDR+3, 142
    POKE BASEADDR+3, 132
    POKE BASEADDR+3, 132
    POKE BASEADDR+3, 250
    POKE BASEADDR+3, 128

Changes made to the text character patterns will be saved permanently when the "Save" command
is executed.


### Register 4 - Device command

**[write-only]**
This register allows a one-shot device command to be executed. The value written is the command
to execute.

|   value   | Description
| --------- | -----------
|    $00    | Load the default configuration values
|    $01    | Load the stored configuration from flash. This automatically happens on power-up.
|    $02    | Save the current configuration to flash
| $10 - $1f | Load one of the built-in character ROMs



#### Saving settings permanently

To permanently save changes to flash, like say you update a text character pattern or want to always
have scanline emulation enabled, then you just execute command `$02`:

    POKE BASEADDR+4, 2

To permanently restore the default settings (text font, monochrome colors, etc) you would need to execute
command `$00` to revert the RAM settings to the defaults and then `$02` to save the change to flash:

    POKE BASEADDR+4, 0
    POKE BASEADDR+4, 2
