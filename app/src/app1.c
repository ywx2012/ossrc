
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <print.h>
#include <timer.h>
#include <shm.h>
#include <fb.h>
#include <draw.h>

int
main(void) {
  char *m = (char *)0x4000000;
  shm_map("shm-1", m);

  *m = 'S';

  struct fb_info fb_info;
  fb_get_info(&fb_info);

  uint32_t *fbbase = (uint32_t *)0xe000000;
  fb_map(fbbase);

  while (1) {
    draw_rect(10, 100, 150, 100, RED, fbbase, &fb_info);
    timer_sleep(1000);
    draw_rect(10, 100, 150, 100, GREEN, fbbase, &fb_info);
    timer_sleep(1000);
    draw_rect(10, 100, 150, 100, BLUE, fbbase, &fb_info);
    timer_sleep(1000);
  }
}
