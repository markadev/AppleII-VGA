#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#define THEN_WAIT_VSYNC (2 << 9)
#define THEN_WAIT_HSYNC (3 << 9)


void vga_init();


#ifdef __cplusplus
}
#endif
