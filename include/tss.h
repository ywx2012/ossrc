
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#pragma once

#include "include/types.h"

#define IO_BITMAP_BYTES (65536 / 8)

struct tss {
  uint32_t reserved1;
  uint64_t rsp0;
  uint64_t rsp1;
  uint64_t rsp2;
  uint64_t reserved2;
  uint64_t ist[7];
  uint32_t reserved3;
  uint32_t reserved4;
  uint16_t reserved5;
  uint16_t io_bitmap_offset;
  uint8_t io_bitmap[IO_BITMAP_BYTES + 1];
} __attribute__((packed));

struct tss_desc {
  uint16_t limit0;
  uint16_t base0;
  uint16_t base1 : 8, type : 4, desc_type : 1, dpl : 2, p : 1;
  uint16_t limit1 : 4, avl : 1, zero0 : 2, g : 1, base2 : 8;
  uint32_t base3;
  uint32_t zero1;
} __attribute__((packed));

extern struct tss tss;
void tss_init();
