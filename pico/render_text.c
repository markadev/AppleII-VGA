#include <pico/stdlib.h>
#include "buffers.h"
#include "render.h"
#include "textfont.h"
#include "vga.h"

static uint_fast32_t text_flasher_mask = 0;
static uint64_t next_flash_tick = 0;

void update_text_flasher()
{
  uint64_t now = time_us_64();
  if (now > next_flash_tick)
  {
    text_flasher_mask ^= 0xff;
    next_flash_tick = now + 250000u;
  }
}

static inline uint_fast8_t __time_critical_func(char_text_bits)(uint_fast8_t ch, uint_fast8_t glyph_line)
{
  uint_fast8_t bits = 0;
  //! Added Alternate ROM support
  if (soft_switches_alt)
  {
    bits = altcharacter_rom[((uint_fast16_t)ch << 3) | glyph_line];
  }
  else
  {
    bits = character_rom[((uint_fast16_t)ch << 3) | glyph_line];
  }

  /*
      Character stored in ram like this
          ______0 - INVERSE       / 1 - Normal
         | _____0 - NOT FLASHING  / 1 - FLASHING
         ||  ___0 - LOWER         / 1 - CAPS
         || |   ___ ASCII Code
         || |  |
      0b 00 000000
  */

  if (ch & 0x80)
  {
    // normal character
    return bits & 0x7f;
  }

  if ((bits & 0x80) == 0)
  {
    // inverse character
    int_fast8_t output = 0;
    if (soft_switches_alt)
    {
      //! Need to check is not MouseText
      if ((ch > 0x3F) & (ch <= 0x5F))
      {
        //! This is MouseText output normal
        output = bits & 0x7f;
      }
      else
      {
        //! it must be Inverse
        output = bits ^ 0x7f;
      }
    }
    else
    {
      //! Normal Characters then inverse
      output = bits ^ 0x7f;
    }
    return output;
  }
  else
  {
    // flashing character
    return (bits ^ text_flasher_mask) & 0x7f;
  }
}

void __time_critical_func(render_text)()
{
  vga_prepare_frame();

  // Skip 48 lines to center vertically
  struct vga_scanline* skip_sl = vga_prepare_scanline();
  for (int i = 0; i < 48; i++)
  {
    skip_sl->data[i] = (uint32_t)THEN_WAIT_HSYNC << 16;
  }
  skip_sl->length = 48;
  vga_submit_scanline(skip_sl);

  if (soft_80col)
  {
    for (int line = 0; line < 24; line++)
    {
      render_text80_line(line);
    }
  }
  else
  {
    for (int line = 0; line < 24; line++)
    {
      render_text_line(line);
    }
  }
}
void __time_critical_func(render_text80_line)(unsigned int line)
{
  //! In 80 Columns mode software adds data in page 1 of text_memory and in page 1 of aux memory
  //! this is how it can get 80 columns, doubling the ram. So need to make sure page does not turn if
  //! 80STORE and PAGE2 are set, in this case we should grab from aux memory.

  const uint8_t* page = (const uint8_t*)(((soft_switches & SOFTSW_PAGE_2) && !soft_80store) ? text_p2 : text_p1);
  const uint8_t* page80 = (const uint8_t*)(((soft_switches & SOFTSW_PAGE_2) && !soft_80store) ? text_p4 : text_p3);
  const uint8_t* line_buf = (const uint8_t*)(page + ((line & 0x7) << 7) + (((line >> 3) & 0x3) * 40));
  const uint8_t* line_80buf = (const uint8_t*)(page80 + ((line & 0x7) << 7) + (((line >> 3) & 0x3) * 40));

  for (uint glyph_line = 0; glyph_line < 8; glyph_line++)
  {
    struct vga_scanline* sl = vga_prepare_scanline();
    uint sl_pos = 0;

    // Pad 40 pixels on the left to center horizontally
    sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl->data[sl_pos++] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels per word

    for (uint col = 0; col < 40;)
    {
      // Grab 14 pixels from the next two characters
      uint32_t bits_a = 0;
      uint32_t bits_b = 0;

      //! Here per column we grab a byte from aux memory and a byte from text_memory, in this order.
      bits_a = char_text_bits(line_80buf[col], glyph_line);
      bits_b = char_text_bits(line_buf[col], glyph_line);
      col++;
      uint32_t bits = (bits_a << 7) | bits_b;

      // Translate each pair of bits into a pair of pixels
      //! Removed the extra timing to make it narrow
      for (int i = 0; i < 7; i++)
      {
        uint32_t pixeldata = (bits & 0x2000) ? (0x1ff) : (0);
        pixeldata |= (bits & 0x1000) ? ((uint32_t)0x1ff) << 16 : ((uint32_t)0) << 16;
        bits <<= 2;
        sl->data[sl_pos] = pixeldata;
        sl_pos++;
      }
    }
    // Pad 40 pixels on the right to center horizontally
    sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl->data[sl_pos++] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels per word

    sl->length = sl_pos;
    sl->repeat_count = 1;
    vga_submit_scanline(sl);
  }
}

void __time_critical_func(render_text_line)(unsigned int line)
{
  const uint8_t* page = (const uint8_t*)(((soft_switches & SOFTSW_PAGE_2) && !soft_80store) ? text_p2 : text_p1);
  const uint8_t* line_buf = (const uint8_t*)(page + ((line & 0x7) << 7) + (((line >> 3) & 0x3) * 40));

  for (uint glyph_line = 0; glyph_line < 8; glyph_line++)
  {
    struct vga_scanline* sl = vga_prepare_scanline();
    uint sl_pos = 0;

    // Pad 40 pixels on the left to center horizontally
    sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl->data[sl_pos++] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels per word

    for (uint col = 0; col < 40;)
    {
      // Grab 14 pixels from the next two characters
      uint32_t bits_a = 0;
      uint32_t bits_b = 0;

      bits_a = char_text_bits(line_buf[col], glyph_line);
      col++;
      bits_b = char_text_bits(line_buf[col], glyph_line);
      col++;
      uint32_t bits = (bits_a << 7) | bits_b;

      // Translate each pair of bits into a pair of pixels
      for (int i = 0; i < 7; i++)
      {
        uint32_t pixeldata = (bits & 0x2000) ? (0x1ff | THEN_EXTEND_1) : (0 | THEN_EXTEND_1);
        pixeldata |= (bits & 0x1000) ? ((uint32_t)0x1ff | THEN_EXTEND_1) << 16 : ((uint32_t)0 | THEN_EXTEND_1) << 16;
        bits <<= 2;
        sl->data[sl_pos] = pixeldata;
        sl_pos++;
      }
    }
    // Pad 40 pixels on the right to center horizontally
    sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl->data[sl_pos++] = (0 | THEN_EXTEND_7) | ((0 | THEN_EXTEND_7) << 16);  // 16 pixels per word
    sl->data[sl_pos++] = (0 | THEN_EXTEND_3) | ((0 | THEN_EXTEND_3) << 16);  // 8 pixels per word

    sl->length = sl_pos;
    sl->repeat_count = 1;

    vga_submit_scanline(sl);
  }
}
