#include "render.h"

#include <pico/stdlib.h>
#include "buffers.h"
#include "colors.h"
#include "textfont/textfont.h"
#include "vga.h"


static uint_fast32_t videx_text_flasher_mask = 0;
static uint64_t videx_next_flash_tick = 0;

void update_videx_text_flasher() {
    uint64_t now = time_us_64();
    if(now > videx_next_flash_tick) {
        videx_text_flasher_mask ^= 0xff;        
        //videx_next_flash_tick = now + 250000u;

        //If videx_blink_cursor is true then
        //videx_blink_mode determines blink rate
        // - false = 1/16th field rate blink 62500u
        // - true = 1/32th field rate blink 31250u
        
        if(videx_blink_mode)
            videx_next_flash_tick = now + 312500u;
        else
            videx_next_flash_tick = now + 625000u;
        
    }
}


static inline uint text_line_to_mem_offset(uint line) {
    return ((line & 0x7) << 7) + (((line >> 3) & 0x3) * 40);
}


static inline uint_fast16_t char_videx_text_bits(uint_fast8_t ch, uint_fast8_t glyph_line, bool has_cursor) {
    
    uint_fast8_t bits;
    if(ch < 0x80){
        bits = videx_normal[((uint_fast16_t)ch << 4) + glyph_line];
    }
    else{
        bits = videx_inverse[((uint_fast16_t)(ch & 0x7f) << 4) + glyph_line];
    }
    
    //If videx_blink_cursor is false then
    //videx_blink_mode determines if there is a cursor displayed (false) or not (true).

    //If videx_blink_cursor is true then
    //videx_blink_mode determines blink rate
    // - false = 1/16th field rate blink
    // - true = 1/32th field rate blink
    if(has_cursor && videx_upperSLOT <= glyph_line && videx_lowerSLOT >= glyph_line) {
        // flashing character
        if(videx_blink_cursor)
            bits ^= videx_text_flasher_mask;
        else if(!videx_blink_mode)
            bits ^= 0xff;
    }

    // VIDEX HAS 8 BIT COL
    return bits;
    //return bits & 0x7f;
}


void render_videx_text() {
    
    vga_prepare_frame();
    // Skip 25 lines to center vertically
    vga_skip_lines(25);

    for(int line = 0; line < 24; line++) {
        render_videx_text_line(line);
    }
    
}

/*
void render_videx_text_line(unsigned int line) {

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

    
    //const uint32_t bg_color = soft_monochrom ? mono_bg_color : ntsc_palette[0];
    const uint32_t bg_color = ntsc_palette[2];
    const uint32_t fg_color = soft_monochrom ? mono_fg_color : ntsc_palette[15];
    uint32_t bits_to_pixelpair[4] = {
        (bg_color << 16) | bg_color,
        (bg_color << 16) | fg_color,
        (fg_color << 16) | bg_color,
        (fg_color << 16) | fg_color,
    };

    for(uint glyph_line = 0; glyph_line < 8; glyph_line++) {
        struct vga_scanline *sl = vga_prepare_scanline();
        uint sl_pos = 0;

        // Pad 40 pixels on the left to center horizontally
        sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels
        sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels
        sl->data[sl_pos++] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels

        
        for(uint col = 0; col < 80;) {
            // Grab 14 pixels from the next two characters. If an aux memory bank was provided (80 column mode is on)
            // then the first character comes from that, otherwise both characters just come from main memory.
            uint_fast8_t char_a = 0x41;//card_videx_getCSLOTXX(col, line);
            uint_fast8_t char_b = char_a;

            uint_fast16_t bits =
                ((uint_fast16_t)char_videx_text_bits(char_b, glyph_line) << 7) | (uint_fast16_t)char_videx_text_bits(char_a, glyph_line);

            // Render each pair of bits into a pair of pixels, least significant bit first
            for(int i = 0; i < 7; i++) {
                sl->data[sl_pos] = bits_to_pixelpair[bits & 0x03];
                sl_pos++;
                bits >>= 2;
            }
        }
        
        if(soft_scanline_emulation) {
            // Just insert a blank scanline between each rendered scanline
            sl->data[sl_pos++] = THEN_WAIT_HSYNC;
        } else {
            sl->repeat_count = 1;
        }
        sl->length = sl_pos;
        vga_submit_scanline(sl);
    }
    
}
*/


void render_videx_text_line(unsigned int line) {
    const uint32_t bg_color = soft_monochrom ? mono_bg_color : ntsc_palette[0];    
    const uint32_t fg_color = soft_monochrom ? mono_fg_color : ntsc_palette[15];
    uint32_t bits_to_pixelpair[4] = {
        (bg_color << 16) | bg_color,
        (bg_color << 16) | fg_color,
        (fg_color << 16) | bg_color,
        (fg_color << 16) | fg_color,
    };

    
    int vstart = ((videx_regSLOT[12] << 8) | videx_regSLOT[13]);
	int vcursor = ((videx_regSLOT[14] << 8) | videx_regSLOT[15]);

    int line_offset;
    int vram_offset;
    bool has_cursor_a;
    bool has_cursor_b;
    uint_fast8_t char_a;
    uint_fast8_t char_b;
    uint_fast16_t bits;

    struct vga_scanline *sl;
    uint sl_pos;
/*
    const uint line_offset = text_line_to_mem_offset(line);

    const uint8_t *page_main = is_page2_display_enabled() ? text_mainmem_page2 : text_mainmem_page1;
    const uint8_t *line_main = page_main + line_offset;

    const uint8_t *line_aux = 0;
    // Read every other character from the aux memory bank in 80 column mode
    const uint8_t *page_aux = is_page2_display_enabled() ? text_auxmem_page2 : text_auxmem_page1;
    line_aux = page_aux + line_offset;
*/

    for(uint glyph_line = 0; glyph_line < 9; glyph_line++) {
        sl = vga_prepare_scanline();
        sl_pos = 0;

        // Pad 40 pixels on the left to center horizontally
        // VIDEX HAS 8 BIT COL 
        //sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels
        //sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels
        //sl->data[sl_pos++] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels

        for(uint col = 0; col < 80;) {
            // Grab 14 pixels from the next two characters. If an aux memory bank was provided (80 column mode is on)
            // then the first character comes from that, otherwise both characters just come from main memory.

/*            
            uint_fast16_t line_offset = (((line * 80) + col) + vstart);
            uint_fast16_t vram_offset = line_offset % 0x800;
            uint_fast8_t is_inverse_a = vcursor == line_offset;
            uint_fast8_t char_a = videx_vram[vram_offset]; 
            uint_fast8_t is_inverse_b = vcursor == line_offset;
            uint_fast8_t char_b = videx_vram[vram_offset]; 

            // VIDEX HAS 8 BIT COL
            uint_fast16_t bits = ((uint_fast16_t)char_videx_text_bits(char_b, glyph_line,is_inverse_b) << 8) | (uint_fast16_t)char_videx_text_bits(char_a, glyph_line, is_inverse_a);
*/

            
            //uint_fast8_t char_a = (line_aux != NULL) ? line_aux[col] : line_main[col++];
            //uint_fast8_t char_b = line_main[col++];
            line_offset = (((line * 80) + col) + vstart);
            vram_offset = line_offset % 0x800;
            has_cursor_a = vcursor == line_offset;
            char_a = videx_vram[vram_offset]; 
            col++;
            
            line_offset = (((line * 80) + col) + vstart);
            vram_offset = line_offset % 0x800;
            has_cursor_b = vcursor == line_offset;
            char_b = videx_vram[vram_offset]; 
            col++;
            

            // VIDEX HAS 8 BIT COL
            bits = (char_videx_text_bits(char_b, glyph_line, has_cursor_b) << 8) | char_videx_text_bits(char_a, glyph_line, has_cursor_a);
            //uint_fast16_t bits = ((uint_fast16_t)char_videx_text_bits(char_b, glyph_line) << 7) | (uint_fast16_t)char_videx_text_bits(char_a, glyph_line);

            // Render each pair of bits into a pair of pixels, least significant bit first
            // VIDEX HAS 8 BIT COL
            for(int i = 0; i < 8; i++) {
            //for(int i = 0; i < 7; i++) {
                sl->data[sl_pos] = bits_to_pixelpair[bits & 0x03];
                sl_pos++;
                bits >>= 2;
            }
        }

        if(soft_scanline_emulation) {
            // Just insert a blank scanline between each rendered scanline
            sl->data[sl_pos++] = THEN_WAIT_HSYNC;
        } else {
            sl->repeat_count = 1;
        }
        sl->length = sl_pos;
        vga_submit_scanline(sl);
    }
}
