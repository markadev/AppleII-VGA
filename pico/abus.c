#include <string.h>
#include <stdio.h>
#include <hardware/pio.h>
#include "config.h"
#include "abus.h"
#include "abus.pio.h"
#include "buffers.h"

#if CONFIG_PIN_APPLEBUS_PHI0 != PHI0_GPIO
#error CONFIG_PIN_APPLEBUS_PHI0 and PHI0_GPIO must be set to the same pin
#endif

enum
{
  ABUS_MAIN_SM = 0,
  ABUS_DEVICE_READ_SM = 1,
};

static void abus_device_read_setup(PIO pio, uint sm)
{
  uint program_offset = pio_add_program(pio, &abus_device_read_program);
  pio_sm_claim(pio, sm);

  pio_sm_config c = abus_device_read_program_get_default_config(program_offset);

  // set the "device selected" pin as the jump pin
  sm_config_set_jmp_pin(&c, CONFIG_PIN_APPLEBUS_DEVSEL);

  // map the OUT pin group to the data signals
  sm_config_set_out_pins(&c, CONFIG_PIN_APPLEBUS_DATA_BASE, 8);

  // map the SET pin group to the Data transceiver control signals
  sm_config_set_set_pins(&c, CONFIG_PIN_APPLEBUS_CONTROL_BASE, 2);

  pio_sm_init(pio, sm, program_offset, &c);

  // All the GPIOs are shared and setup by the main program
}

static void abus_main_setup(PIO pio, uint sm)
{
  uint program_offset = pio_add_program(pio, &abus_program);
  pio_sm_claim(pio, sm);

  pio_sm_config c = abus_program_get_default_config(program_offset);

  // set the bus R/W pin as the jump pin
  sm_config_set_jmp_pin(&c, CONFIG_PIN_APPLEBUS_RW);

  // map the IN pin group to the data signals
  sm_config_set_in_pins(&c, CONFIG_PIN_APPLEBUS_DATA_BASE);

  // map the SET pin group to the bus transceiver enable signals
  sm_config_set_set_pins(&c, CONFIG_PIN_APPLEBUS_CONTROL_BASE + 1, 3);

  // configure left shift into ISR & autopush every 26 bits
  sm_config_set_in_shift(&c, false, true, 26);

  pio_sm_init(pio, sm, program_offset, &c);

  // configure the GPIOs
  // Ensure all transceivers will start disabled, with Data transceiver direction set to 'in'
  pio_sm_set_pins_with_mask(
      pio, sm, (uint32_t)0xe << CONFIG_PIN_APPLEBUS_CONTROL_BASE, (uint32_t)0xf << CONFIG_PIN_APPLEBUS_CONTROL_BASE);
  pio_sm_set_pindirs_with_mask(pio,
                               sm,
                               (0xf << CONFIG_PIN_APPLEBUS_CONTROL_BASE),
                               (1 << CONFIG_PIN_APPLEBUS_PHI0) | (0xf << CONFIG_PIN_APPLEBUS_CONTROL_BASE) |
                                   (0x3ff << CONFIG_PIN_APPLEBUS_DATA_BASE));

  // Disable input synchronization on input pins that are sampled at known stable times
  // to shave off two clock cycles of input latency
  pio->input_sync_bypass |= (0x3ff << CONFIG_PIN_APPLEBUS_DATA_BASE);

  pio_gpio_init(pio, CONFIG_PIN_APPLEBUS_PHI0);
  gpio_set_pulls(CONFIG_PIN_APPLEBUS_PHI0, false, false);
  for (int pin = CONFIG_PIN_APPLEBUS_CONTROL_BASE; pin < CONFIG_PIN_APPLEBUS_CONTROL_BASE + 4; pin++)
  {
    pio_gpio_init(pio, pin);
  }
  for (int pin = CONFIG_PIN_APPLEBUS_DATA_BASE; pin < CONFIG_PIN_APPLEBUS_DATA_BASE + 10; pin++)
  {
    pio_gpio_init(pio, pin);
    gpio_set_pulls(pin, false, false);
  }
}

void abus_init()
{
  abus_device_read_setup(CONFIG_ABUS_PIO, ABUS_DEVICE_READ_SM);
  abus_main_setup(CONFIG_ABUS_PIO, ABUS_MAIN_SM);

  pio_enable_sm_mask_in_sync(CONFIG_ABUS_PIO, (1 << ABUS_MAIN_SM) | (1 << ABUS_DEVICE_READ_SM));
  soft_80col = 0;
  soft_80store = 0;
}

static void __time_critical_func(shadow_memory)(uint address, uint32_t value)
{

  //! Soft switches
  // Shadow the soft-switches by observing all read & write bus cycles
  if (address == 0xc003)
  {
    soft_ramrd = ((uint32_t)SOFTSW_READ_AUX);
  }
  if (address == 0xc002)
  {
    soft_ramrd = ((uint32_t)SOFTSW_READ_MAIN);
  }
  if (address == 0xc005)
  {
    soft_ramwrt = ((uint32_t)SOFTSW_WRITE_AUX);
  }
  if (address == 0xc004)
  {
    soft_ramwrt = ((uint32_t)SOFTSW_WRITE_MAIN);
  }

  if (address == 0xc07e)
  {
    soft_ioudis = ((uint32_t)SOFTSW_IOUDIS_ON);
  }
  if (address == 0xc07f)
  {
    soft_ioudis = ((uint32_t)SOFTSW_IOUDIS_OFF);
  }

  if (address == 0xc05e)
  {
    soft_an3 = ((uint32_t)SOFTSW_AN3_OFF);
    soft_dhires = ((uint32_t)SOFTSW_DHIRES_ON);
  }
  if (address == 0xc05f)
  {
    soft_an3 = ((uint32_t)SOFTSW_AN3_ON);
    soft_dhires = ((uint32_t)SOFTSW_DHIRES_OFF);
  }
  if (address == 0xc00c)
  {
    soft_80col = ((uint32_t)SOFTSW_80COL_OFF);
  }
  if (address == 0xc00d)
  {
    soft_80col = ((uint32_t)SOFTSW_80COL_ON);
  }

  if (address == 0xC000)
  {
    soft_80store = ((uint32_t)SOFTSW_80STORE_OFF);
  }

  if (address == 0xc001)
  {
    soft_80store = ((uint32_t)SOFTSW_80STORE_ON);
  }

  if ((address & 0xfff8) == 0xc050)
  {
    switch (address & 7)
    {
      case 0:
        soft_switches &= ~((uint32_t)SOFTSW_TEXT_MODE);
        break;
      case 1:
        soft_switches |= SOFTSW_TEXT_MODE;
        break;
      case 2:
        soft_switches &= ~((uint32_t)SOFTSW_MIX_MODE);
        break;
      case 3:
        soft_switches |= SOFTSW_MIX_MODE;
        break;
      case 4:
        soft_switches &= ~((uint32_t)SOFTSW_PAGE_2);
        break;
      case 5:
        soft_switches |= SOFTSW_PAGE_2;
        break;
      case 6:
        soft_switches &= ~((uint32_t)SOFTSW_HIRES_MODE);
        break;
      case 7:
        soft_switches |= SOFTSW_HIRES_MODE;
        break;
    }
  }
  else if ((address & 0xfffe) == 0xc00e)
  {
    //! ALTCHAR Softswitch access
    switch (address & 1)
    {
      case 0:
      {
        soft_switches_alt = ((uint32_t)SOFTSW_ALTCHAR_OFF);
        break;
      }
      case 1:
      {
        soft_switches_alt = ((uint32_t)SOFTSW_ALTCHAR_ON);
        break;
      }
      default:
      {
        soft_switches_alt == ((uint32_t)SOFTSW_ALTCHAR_OFF);
        break;
      }
    }
  }

  // Shadow parts of the Apple's memory by observing the bus write cycles
  if ((value & (1u << CONFIG_PIN_APPLEBUS_RW - CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)
  {
    if ((address >= 0x0400u) && (address < 0x0800u))
    {
      // if 80store is on and PAGE two is activated store it in aux or ramwrt to aux is set
      if (soft_80store)
      {
        if ((soft_switches & SOFTSW_PAGE_2))
        {
          if (soft_ramwrt)
          {
            aux_memory[address - 0x400] = value & 0xff;
          }
          else
          {
            if ((address >= 0x0400u) && (address < 0x0800u))
            {
              aux_memory[address - 0x400] = value & 0xff;
            }
            else
            {
              text_memory[address - 0x400] = value & 0xff;
            }
          }
        }
        else
        {
          if (soft_ramwrt)
          {
            if ((address >= 0x0400u) && (address < 0x0800u))
            {
              text_memory[address - 0x400] = value & 0xff;
            }
            else
            {
              aux_memory[address - 0x400] = value & 0xff;
            }
          }
          else
          {
            text_memory[address - 0x400] = value & 0xff;
          }
        }
      }
      else if (soft_ramwrt)
      {
        text_memory[address - 0x400] = value & 0xff;
      }
      else
      {
        text_memory[address - 0x400] = value & 0xff;
      }
    }
    else if ((address >= 0x0800u) && (address < 0x0C00u))
    {
      text_memory[address - 0x400] = value & 0xff;
    }
    else if ((address >= 0x2000) && (address < 0x6000))
    {
      // Mirror Video Memory from MAIN & AUX banks
      //! From this:
      // https://gswv.apple2.org.za/USA2WUG/IIe.Dev.TechNotes/APPLE2E3.TXT

      if (soft_80store)
      {
        if (soft_switches & SOFTSW_PAGE_2)
        {
          if ((soft_switches & SOFTSW_HIRES_MODE))
          {
            if (soft_ramwrt)
            {
              // Both Pages in Aux
              dhires_aux_memory[address - 0x2000] = value & 0xff;
            }
            else
            {
              if ((address >= 0x2000) && (address < 0x4000))
              {
                //! Page 1 of Aux
                dhires_aux_memory[address - 0x2000] = value & 0xff;
              }
              else
              {
                //! Page 2 of Hires
                hires_memory[address - 0x2000] = value & 0xff;
              }
            }
          }
          else
          {
            if (soft_ramwrt)
            {
              //! Both Pages in Aux
              dhires_aux_memory[address - 0x2000] = value & 0xff;
            }
            else
            {
              //! Both Pages
              hires_memory[address - 0x2000] = value & 0xff;
            }
          }
        }
        else
        {
          if ((soft_switches & SOFTSW_HIRES_MODE))
          {
            if (soft_ramwrt)
            {
              if ((address >= 0x2000) && (address < 0x4000))
              {
                //! Page 1 of Hires
                hires_memory[address - 0x2000] = value & 0xff;
              }
              else
              {
                //! Page 2 of Hires
                dhires_aux_memory[address - 0x2000] = value & 0xff;
              }
            }
            else
            {
              //! Both Pages
              hires_memory[address - 0x2000] = value & 0xff;
            }
          }
          else
          {
            if (soft_ramwrt)
            {
              //! Both Pages
              dhires_aux_memory[address - 0x2000] = value & 0xff;
            }
            else
            {
              //! Both Pages
              hires_memory[address - 0x2000] = value & 0xff;
            }
          }
        }
      }
      else if (soft_ramwrt)
      {
        // AUX PAGE X1 and X2
        dhires_aux_memory[address - 0x2000] = value & 0xff;
      }
      else
      {
        // MAIN PAGE 1 and 2
        hires_memory[address - 0x2000] = value & 0xff;
      }
    }
  }
}

void __time_critical_func(abus_loop)()
{
  while (1)
  {
    uint32_t value = pio_sm_get_blocking(CONFIG_ABUS_PIO, ABUS_MAIN_SM);
    uint address = (value >> 10) & 0xffff;

    if ((value & (1u << CONFIG_PIN_APPLEBUS_DEVSEL - CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)
    {
      if ((value & (1u << CONFIG_PIN_APPLEBUS_RW - CONFIG_PIN_APPLEBUS_DATA_BASE)) != 0)
      {
        // device read access
        pio_sm_put_blocking(CONFIG_ABUS_PIO, ABUS_DEVICE_READ_SM, address & 0xf);
      }
    }

    shadow_memory(address, value);
  }
}
