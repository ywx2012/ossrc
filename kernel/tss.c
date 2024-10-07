
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <string.h>
#include "include/tss.h"
#include "include/segment.h"
#include "include/sched.h"

extern unsigned long gdt[64];
struct tss tss;

void tss_init() {
  memset(&tss, 0, sizeof(tss));
  tss.io_bitmap_offset = __builtin_offsetof(struct tss, io_bitmap);
  tss.io_bitmap[IO_BITMAP_BYTES] = ~0;
  tss.rsp0 = current->kstack;

  struct tss_desc* desc = (struct tss_desc*)&gdt[GDT_TSS_ENTRY];
  memset(desc, 0, sizeof(struct tss_desc));
  desc->limit0 = sizeof(tss) & 0xffff;
  desc->base0 = (unsigned long)(&tss) & 0xffff;
  desc->base1 = ((unsigned long)(&tss) >> 16) & 0xff;
  desc->type = 0x9;
  desc->p = 1;
  desc->limit1 = (sizeof(tss) >> 16) & 0xf;
  desc->base2 = ((unsigned long)(&tss) >> 24) & 0xff;
  desc->base3 = (unsigned long)(&tss) >> 32;

  // just for test
  //memset(tss.io_bitmap, 0xff, IO_BITMAP_BYTES);

  __asm__ ("ltr %w0" : : "r"(GDT_TSS_ENTRY << 3));
}
