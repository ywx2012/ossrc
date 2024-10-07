
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#pragma once

#include "include/types.h"

#define E820MAX       32
#define E820_RAM      1
#define E820_RESERVED 2

#define MAX_PAGES (1024 * 1024)
#define KERNEL_PAGE_NUM (1024 * 16)
#define PAGE_SIZE 4096

#define PAGE_OFFSET 0xffff888000000000
#define VA(x) ((void*)((unsigned long)(x) + PAGE_OFFSET))
#define PA(x) ((unsigned long)(x) - PAGE_OFFSET)

#define TASK0_PML4 0x14000

extern unsigned long mem_size;
extern char pages[MAX_PAGES];

struct e820entry {
  uint64_t addr;
  uint64_t size;
  uint32_t type;
} __attribute__((packed));

struct e820map {
  uint32_t nr_entry;
  struct e820entry map[E820MAX];
};

void mm_init();
unsigned long alloc_page();
void free_page(unsigned long addr);
void* malloc(int size);
void free(void* obj);
void map_range(unsigned long pml4_pa, unsigned long from_va, unsigned long to_pa, char us, long npage);
