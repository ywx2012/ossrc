#pragma once

#include <stdint.h>

struct fb_info {
  uint16_t xres;
  uint16_t yres;
};

int fb_get_info(struct fb_info *fb_info);
int fb_map(uintptr_t va);
