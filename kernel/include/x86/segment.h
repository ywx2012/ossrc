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
  uint32_t base;
} __attribute__((packed));

struct gate {
  uintptr_t offset: 16;
  uintptr_t segment_selector: 16;
  uintptr_t zero: 8;
  uintptr_t type: 3;
  uintptr_t size: 1;
  uintptr_t s: 1;
  uintptr_t dpl: 2;
  uintptr_t present: 1;
  uintptr_t offset_l: 16;
} __attribute__((packed));

struct tss {
  uintptr_t prev_task_link: 16;
  uintptr_t reserved_0: 16;
  uintptr_t esp0;
  uintptr_t ss0: 16;
  uintptr_t reserved_1: 16;
  uintptr_t esp1;
  uintptr_t ss1: 16;
  uintptr_t reserved_2: 16;
  uintptr_t esp2;
  uintptr_t ss2: 16;
  uintptr_t reserved_3: 16;
  uintptr_t cr3;
  uintptr_t eip;
  uintptr_t eflags;
  uintptr_t eax;
  uintptr_t ecx;
  uintptr_t edx;
  uintptr_t ebx;
  uintptr_t esp;
  uintptr_t ebp;
  uintptr_t esi;
  uintptr_t edi;
  uintptr_t es: 16;
  uintptr_t reserved_4: 16;
  uintptr_t cs: 16;
  uintptr_t reserved_5: 16;
  uintptr_t ss: 16;
  uintptr_t reserved_6: 16;
  uintptr_t ds: 16;
  uintptr_t reserved_7: 16;
  uintptr_t fs: 16;
  uintptr_t reserved_8: 16;
  uintptr_t gs: 16;
  uintptr_t reserved_9: 16;
  uintptr_t lds_segment_selector: 16;
  uintptr_t reserved_10: 16;
  uintptr_t trap: 1;
  uintptr_t reserved_11: 15;
  uintptr_t io_base: 16;
  uintptr_t ssp;
  uint8_t iomap[N_IOPORTS/sizeof(uint8_t)+1];
} __attribute__((packed));

#define SEGMENT(base,limit,s,type,dpl,g) \
  { (limit)&0xFFFF,(base)&0xFFFF,        \
    ((base)>>16)&0xFF,(type),(s),(dpl),  \
    1,((limit)>>16)&0xF,0,0,1,(g),       \
    (uint8_t)(((base)>>24)&0xFF) }

#define CODESEG(dpl) SEGMENT(0,0xFFFFF,1,0xb,(dpl)&0x3,1)
#define DATASEG(dpl) SEGMENT(0,0xFFFFF,1,0x3,(dpl)&0x3,1)
#define TASKSEG(base,limit) SEGMENT((base),(limit),0,0x9,0,0)

#define GATE_TYPE_INT  0x6
#define GATE_TYPE_TRAP 0x7
