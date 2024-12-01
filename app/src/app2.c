
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <stdint.h>
#include <print.h>
#include <std.h>
#include <draw.h>

void draw_char(uint64_t fbaddr, struct mode_info* mode_info, int color) {
  // H
  draw_en(10, 2, 0, color, fbaddr, mode_info);
  // e
  draw_en(20, 2, 1, color, fbaddr, mode_info);
  // l
  draw_en(30, 2, 2, color, fbaddr, mode_info);
  // l
  draw_en(40, 2, 2, color, fbaddr, mode_info);
  // o
  draw_en(50, 2, 3, color, fbaddr, mode_info);
  // ,
  draw_en(60, 2, 4, color, fbaddr, mode_info);

  //zhong
  draw_zh(70, 2, 0, color, fbaddr, mode_info);
  //guo
  draw_zh(90, 2, 1, color, fbaddr, mode_info);
  // .
  draw_en(110, 2, 5, color, fbaddr, mode_info);
}

int main() {
  void* m = shm_open("shm-1");

  struct mode_info mode_info;
  get_mode_info(&mode_info);

  unsigned long fbbase = fbmap();
  draw_char(fbbase, &mode_info, RED | GREEN);

  unsigned long i = 0;
  while (++i) {
    if (i % 2) {
      draw_char(fbbase, &mode_info, RED | GREEN);
    } else {
      draw_char(fbbase, &mode_info, RED | BLUE);
    }
    print(*(char*)m);
    sleep(1000);
  }
}
