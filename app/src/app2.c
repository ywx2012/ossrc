
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <print.h>
#include <shm.h>
#include <fb.h>
#include <timer.h>
#include <draw.h>

static
void draw_char(uint32_t *fbaddr, struct fb_info* fb_info, uint32_t color) {
  // H
  draw_en(10, 2, 0, color, fbaddr, fb_info);
  // e
  draw_en(20, 2, 1, color, fbaddr, fb_info);
  // l
  draw_en(30, 2, 2, color, fbaddr, fb_info);
  // l
  draw_en(40, 2, 2, color, fbaddr, fb_info);
  // o
  draw_en(50, 2, 3, color, fbaddr, fb_info);
  // ,
  draw_en(60, 2, 4, color, fbaddr, fb_info);

  //zhong
  draw_zh(70, 2, 0, color, fbaddr, fb_info);
  //guo
  draw_zh(90, 2, 1, color, fbaddr, fb_info);
  // .
  draw_en(110, 2, 5, color, fbaddr, fb_info);
}

int
main(void) {
  char *m = (char *)0x4000000;
  shm_map("shm-1", m);

  struct fb_info fb_info;
  fb_get_info(&fb_info);

  uint32_t *fbbase = (uint32_t *)0xe000000;
  fb_map(fbbase);
  draw_char(fbbase, &fb_info, RED | GREEN);

  unsigned long i = 0;
  while (++i) {
    if (i % 2) {
      draw_char(fbbase, &fb_info, RED | GREEN);
    } else {
      draw_char(fbbase, &fb_info, RED | BLUE);
    }
    print(*m);
    timer_sleep(1000);
  }

  return 0;
}
