
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#pragma once

#include <stdint.h>

struct mode_info {
  uint32_t fbbase;
  uint16_t hres;
  uint16_t vres;
  uint8_t bpp;
};

void vesa_init();
