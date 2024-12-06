
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <draw.h>

extern const uint8_t fonts_zh[2][32];
extern const uint8_t fonts_en[6][16];

void draw_en(int origin_x, int origin_y, int index, uint32_t color, uint32_t *fbbase, struct fb_info* fb_info) {
  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 1; x++) {
      int pos = y * 1 + x;

      uint8_t byte = fonts_en[index][pos];
      int n = 0;
      while (n++ < 8) {
        uint8_t bit = (byte >> (8 - n)) & 0x1;
        if (bit == 1) {
          *(fbbase + (origin_y + y) * fb_info->xres + (origin_x + x * 8 + (n - 1))) = color;
        }
      }
    }
  }
}

void draw_zh(int origin_x, int origin_y, int index, uint32_t color, uint32_t *fbbase, struct fb_info* fb_info) {
  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 2; x++) {
      int pos = y * 2 + x;

      uint8_t byte = fonts_zh[index][pos];
      int n = 0;
      while (n++ < 8) {
        uint8_t bit = (byte >> (8 - n)) & 0x1;
        if (bit == 1) {
          *(fbbase + (origin_y + y) * fb_info->xres + (origin_x + x * 8 + (n - 1))) = color;
        }
      }
    }
  }
}

void draw_rect(int origin_x, int origin_y, int l, int w, uint32_t color, uint32_t *fbbase, struct fb_info* fb_info) {
  for (int i = origin_x; i < origin_x + l; i++) {
    *(fbbase + origin_y * fb_info->xres + i) = color;
    *(fbbase + (origin_y + w) * fb_info->xres + i) = color;
  }

  for (int i = origin_y; i < origin_y + w; i++) {
    *(fbbase + i * fb_info->xres + origin_x) = color;
    *(fbbase + i * fb_info->xres + origin_x + l) = color;
  }
}
