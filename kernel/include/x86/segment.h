#pragma once

#include <stdint.h>

#define N_IOPORTS (1<<16)

struct segment {
  uint64_t limit_l: 16;
  uint64_t base_l: 16;
  uint64_t base_h: 8;
  uint64_t type: 4;
  uint64_t s: 1;
  uint64_t dpl: 2;
  uint64_t present: 1;
  uint64_t limit_h: 4;
  uint64_t avail: 1;
  uint64_t l: 1;
  uint64_t db: 1;
  uint64_t granularity: 1;
  uint64_t base_e: 8;
} __attribute__((packed, aligned(8)));

struct dtr {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed));

struct tss {
  uintptr_t reserved_0: 32;
  uintptr_t rsp[3];
  uintptr_t ist[8];
  uintptr_t reserved_1;
  uintptr_t reserved_2: 16;
  uintptr_t io_base: 16;
  uint8_t iomap[N_IOPORTS/sizeof(uint8_t)+1];
} __attribute__((packed));

#define SEGMENT(base,limit,s,type,dpl,g) \
  { (limit)&0xFFFF,(base)&0xFFFF,        \
    ((base)>>16)&0xFF,(type),(s),(dpl),  \
    1,((limit)>>16)&0xF,0,1,0,(g),       \
    ((base)>>24)&0xFF }

#define CODESEG(dpl) SEGMENT(0,0xFFFFF,1,0x9,(dpl)&0x3,1)
#define DATASEG(dpl) SEGMENT(0,0xFFFFF,1,0x3,(dpl)&0x3,1)
#define TASKSEG(base,limit) SEGMENT((base),(limit),0,0x9,0,0)
