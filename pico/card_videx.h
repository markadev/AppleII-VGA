#pragma once

#ifndef CARD_VIDEX
#define CARD_VIDEX

#include <stdint.h>
#include <stdbool.h>
/*
//notes:

// To turn on/off 80 columns mode:
// Write at 0xC058 or 0xC059
//		card_videx_80col = (addr == 0xc059);
//
// To Control the card:
//
// IOSEL // 0xC300-C3FFF - slot 3
// Read
//		r = videxcard.getRomIoSel(addr & 0x1FF);
//		card_videx_mem_on = true;
// Write
//		card_videx_mem_on = true;
//
// DEVSEL // 0xC0B0-C0BF
// Read
//		r = videxcard.getC0SLOTX(addr);
//
// Write
//		videxcard.putC0SLOTX(addr, val);
//
// IOSTB (0xC800-0xCFFF)
// Read
//		if (((addr >> 9) & 0b11) == 0b11) card_videx_mem_on = false;
//
//		if (card_videx_mem_on && addr >= 0xC800 && addr <= 0xCDFF)
//			r = videxcard.getSLOTC8XX(addr & 0x7ff);
//
// Write
//		if (((addr >> 9) & 0b11) == 0b11) card_videx_mem_on = false;
//
//		if (card_videx_mem_on && addr >= 0xC800 && addr <= 0xCDFF)
//			videxcard.putSLOTC8XX(addr - 0xC800, val);
//
// To Render:
//
//
//for (int y = 0; y < 25; y++) {
//	for (int x = 0; x < 80; x++) {
//
//		int vkeycode = videxcard.getCSLOTXX(x, y); // get ascii code from memory
//		bool vinverse = videxcard.isCursorPosition(x, y); // check cursor position to inverse
//
//		for (int i = 0; i < 9; i++) {
//
//			int video_bits = videxcard.getVideoBits(vkeycode, i); // get bits for the current char/scanline
//
//			if (vinverse)
//				display_bit_80col(renderer, x, y, ~video_bits, i); // render char/scanline
//			else
//				display_bit_80col(renderer, x, y, video_bits, i); // render char/scanline
//
//		}
//	}
//}
*/

extern const uint8_t videx_rom[0x400];
extern uint8_t videx_chars[0xfff];  // 0x0-0x7ff rom 1 | 0x800-0xfff rom2 character rom (12x8) Matrix)
extern uint8_t videx_vram[2048];    // 0x0800 at CC00-CDFF in 4 banks of 0x1FF

// internal variables
extern uint8_t videx_bankSLOT;
extern uint8_t videx_regvalSLOT;                  // active memory bank, selected register
extern uint8_t videx_regSLOT[17];                 // registers of the CRT-controller
extern uint8_t videx_upperSLOT, videx_lowerSLOT;  // cursor size
extern uint8_t videx_blink_mode, videx_blink_cursor;
//*******************************************************
// Registers
//    register r/w     normal value    Name
//    00:      w       7B              Horiz. total
//    01:      w       50              Horiz. displayed
//    02:      w       62              Horiz. sync pos
//    03:      w       29              Horiz. sync width
//    04:      w       1B              Vert. total
//    05:      w       08              Vert. adjust
//    06:      w       18              Vert. displayed
//    07:      w       19              Vert. sync pos
//    08:      w       00              Interlaced
//    09:      w       08              Max. scan line
//    10:0A    w       C0              Cursor upper
//    11:0B    w       08              Cursor lower
//    12:0C    w       00              Startpos Hi
//    13:0D    w       00              Startpos Lo
//    14:0E    r/w     00              Cursor Hi
//    15:0F    r/w     00              Cursor Lo
//    16:10    r       00              Lightpen Hi
//    17:11    r       00              Lightpen Lo
//
// The registers are addressed as follows:
//    To write                  To read
//    LDA #$<register>          LDA #$<register>
//    STA $C0B0                 STA $C0B0
//    LDA #$<value>             LDA $C0B1
//    STA $C0B1
//*******************************************************

// 0x0400 at C800-CBFF
int card_videx_getC0SLOTX(int adr);

// 0x0400 at C800-CBFF
void card_videx_putC0SLOTX(int adr, int value);

// IOSEL READS ROM
// when reading using iosel, adds bit at A9
uint8_t card_videx_getRomIoSel(int addr);

// Read Video Memory using position x, y
int8_t card_videx_getCSLOTXX(int col, int row);

// Read Character Rom (Char/scanline)
int8_t card_videx_getVideoBits(int8_t keycode, int line);

// Is cursor at defined x, y?
bool card_videx_isCursorPosition(int col, int row);
// IOSTB - Get Rom or VRam
int card_videx_getSLOTC8XX(int adr);

// IOSTB - set VRam
void card_videx_putSLOTC8XX(int adr, int value);

void card_videx_card_videx_init();

void card_videx_modifySLOT();
#endif