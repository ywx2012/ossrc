
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <stddef.h>
#include <timer.h>
#include <shm.h>

#define RED 0xff0000
#define GREEN 0x00ff00
#define BLUE 0x0000ff

static struct shm_info fb_info;
static uint32_t *fbbase = (uint32_t *)0xe000000;

static
void
draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (uint16_t i=x; i<x+width; ++i) {
    fbbase[fb_info.xres * y + i] = color;
    fbbase[fb_info.xres * (y+height) + i] = color;
  }

  for (uint16_t i=y; i<y+height; ++i) {
    fbbase[fb_info.xres * i + x] = color;
    fbbase[fb_info.xres * i + x + width] = color;
  }
}

int
main(void) {
  char *m = (char *)0x4000000;
  shm_map("shm-1", m);
  *m = 'S';
  shm_get_info("fb-1", &fb_info);
  shm_map("fb-1", fbbase);
  uint32_t color[] = {RED, GREEN, BLUE};

  for(;;) {
    for (size_t i=0; i<sizeof(color)/sizeof(color[0]); ++i) {
      draw_rect(10, 100, 150, 100, color[i]);
      timer_sleep(1000);
    }
  }
}
