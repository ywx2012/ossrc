
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <stdint.h>
#include <print.h>
#include <std.h>
#include <draw.h>

int main() {
  void* m = shm_open("shm-1");
  *(char*)m = 'S';

  struct fb_info fb_info;
  get_fb_info(&fb_info);

  unsigned long fbbase = fbmap();

  while (1) {
    draw_rect(10, 100, 150, 100, RED, fbbase, &fb_info);
    sleep(1000);
    draw_rect(10, 100, 150, 100, GREEN, fbbase, &fb_info);
    sleep(1000);
    draw_rect(10, 100, 150, 100, BLUE, fbbase, &fb_info);
    sleep(1000);
  }
}
