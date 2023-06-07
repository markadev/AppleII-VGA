#include <pico/stdlib.h>
#include <pico/multicore.h>
#include "abus.h"
#include "config.h"
#include "render.h"
#include "vga.h"

static void core1_main()
{
  vga_init();
  render_init();
  render_loop();
}

int main()
{
  // Adjust system clock for better dividing into other clocks
  set_sys_clock_khz(CONFIG_SYSCLOCK * 1000, true); 
  // Setup the on-board LED for debugging
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
  multicore_launch_core1(core1_main);

  abus_init();
  abus_loop();
}
