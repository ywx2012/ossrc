
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <string.h>
#include <tss.h>
#include <segment.h>
#include <sched.h>
#include <bsp.h>

struct tss tss;

void tss_init() {
  memset(&tss, 0, sizeof(tss));
  tss.io_bitmap_offset = __builtin_offsetof(struct tss, io_bitmap);
  tss.io_bitmap[IO_BITMAP_BYTES] = ~0;
  tss.rsp0 = current->kstack;

  struct dtr GDTR __attribute__((aligned(16))) = {
    .base = (uintptr_t)GDT,
    .limit = sizeof(GDT) - 1,
  };
  __asm__("lgdt %0" : : "m" (GDTR));

  uintptr_t base = (uintptr_t)&tss;
  struct segment tssd = TASKSEG(base, sizeof(tss));
  GDT[GDT_TSS_ENTRY] = tssd;
  *(uint64_t *)(GDT+GDT_TSS_ENTRY+1) = (base>>32);

  // just for test
  //memset(tss.io_bitmap, 0xff, IO_BITMAP_BYTES);

  __asm__ ("ltr %w0" : : "r"(GDT_TSS_ENTRY << 3));
}
