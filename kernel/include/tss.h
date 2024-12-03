
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#pragma once

#include <stdint.h>

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

extern struct tss tss;
void tss_init();
