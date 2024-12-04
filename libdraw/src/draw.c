
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <draw.h>

extern const char fonts_zh[2][32];
extern const char fonts_en[6][16];

void draw_en(int origin_x, int origin_y, int index, int color, unsigned long fbbase, struct fb_info* fb_info) {
  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 1; x++) {
      int pos = y * 1 + x;

      char byte = fonts_en[index][pos];
      int n = 0;
      while (n++ < 8) {
        char bit = (byte >> (8 - n)) & 0x1;
        if (bit == 1) {
          *((uint32_t*)fbbase + (origin_y + y) * fb_info->xres + (origin_x + x * 8 + (n - 1))) = color;
        }
      }
    }
  }
}

void draw_zh(int origin_x, int origin_y, int index, int color, unsigned long fbbase, struct fb_info* fb_info) {
  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 2; x++) {
      int pos = y * 2 + x;

      char byte = fonts_zh[index][pos];
      int n = 0;
      while (n++ < 8) {
        char bit = (byte >> (8 - n)) & 0x1;
        if (bit == 1) {
          *((uint32_t*)fbbase + (origin_y + y) * fb_info->xres + (origin_x + x * 8 + (n - 1))) = color;
        }
      }
    }
  }
}

void draw_rect(int origin_x, int origin_y, int l, int w, int color, unsigned long fbbase, 
               struct fb_info* fb_info) {
  for (int i = origin_x; i < origin_x + l; i++) {
    *((uint32_t*)fbbase + origin_y * fb_info->xres + i) = color;
    *((uint32_t*)fbbase + (origin_y + w) * fb_info->xres + i) = color;
  }

  for (int i = origin_y; i < origin_y + w; i++) {
    *((uint32_t*)fbbase + i * fb_info->xres + origin_x) = color;
    *((uint32_t*)fbbase + i * fb_info->xres + origin_x + l) = color;
  }
}


void clear(unsigned long fbbase, struct fb_info* fb_info) {
  for (int y = 0; y < fb_info->yres; y++) {
    for (int x = 0; x < fb_info->xres; x++) {
      *((uint32_t*)fbbase + y * fb_info->xres + x) = 0;
    }
  }
}
