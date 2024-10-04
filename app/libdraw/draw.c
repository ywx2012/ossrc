
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include "include/vesa.h"
#include "app/libdraw/draw.h"

extern const char fonts_zh[2][32];
extern const char fonts_en[6][16];

void draw_en(int origin_x, int origin_y, int index, int color, unsigned long fbbase, struct mode_info* mode_info) {
  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 1; x++) {
      int pos = y * 1 + x;

      char byte = fonts_en[index][pos];
      int n = 0;
      while (n++ < 8) {
        char bit = (byte >> (8 - n)) & 0x1;
        if (bit == 1) {
          *((uint32_t*)fbbase + (origin_y + y) * mode_info->hres + (origin_x + x * 8 + (n - 1))) = color;
        }
      }
    }
  }
}

void draw_zh(int origin_x, int origin_y, int index, int color, unsigned long fbbase, struct mode_info* mode_info) {
  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 2; x++) {
      int pos = y * 2 + x;

      char byte = fonts_zh[index][pos];
      int n = 0;
      while (n++ < 8) {
        char bit = (byte >> (8 - n)) & 0x1;
        if (bit == 1) {
          *((uint32_t*)fbbase + (origin_y + y) * mode_info->hres + (origin_x + x * 8 + (n - 1))) = color;
        }
      }
    }
  }
}

void draw_rect(int origin_x, int origin_y, int l, int w, int color, unsigned long fbbase, 
               struct mode_info* mode_info) {
  for (int i = origin_x; i < origin_x + l; i++) {
    *((uint32_t*)fbbase + origin_y * mode_info->hres + i) = color;
    *((uint32_t*)fbbase + (origin_y + w) * mode_info->hres + i) = color;
  }

  for (int i = origin_y; i < origin_y + w; i++) {
    *((uint32_t*)fbbase + i * mode_info->hres + origin_x) = color;
    *((uint32_t*)fbbase + i * mode_info->hres + origin_x + l) = color;
  }
}


void clear(unsigned long fbbase, struct mode_info* mode_info) {
  for (int y = 0; y < mode_info->vres; y++) {
    for (int x = 0; x < mode_info->hres; x++) {
      *((uint32_t*)fbbase + y * mode_info->hres + x) = 0;
    }
  }
}
