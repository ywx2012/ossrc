
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <fb.h>

#define BLUE 0xff0000
#define GREEN 0xff00
#define RED 0xff

void draw_zh(int origin_x, int origin_y, int index, uint32_t color, uint32_t *fbbase, struct fb_info* fb_info);
void draw_en(int origin_x, int origin_y, int index, uint32_t color, uint32_t *fbbase, struct fb_info* fb_info);
void draw_rect(int origin_x, int origin_y, int l, int w, uint32_t color, uint32_t *fbbase, struct fb_info* fb_info);
