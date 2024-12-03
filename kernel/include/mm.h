
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#pragma once

#include <stdint.h>

#define MAX_PAGES (1024 * 1024)
#define KERNEL_PAGE_NUM (1024 * 16)
#define PAGE_SIZE 4096

#define PAGE_MASK 0xffffffffff000
#define PAGE_OFFSET 0xffff800000000000
#define VA(x) ((void*)((unsigned long)(x) + PAGE_OFFSET))
#define PA(x) ((unsigned long)(x) - PAGE_OFFSET)

extern char pml4[PAGE_SIZE];
#define TASK0_PML4 (((unsigned long)pml4)-PAGE_OFFSET)

extern unsigned long mem_size;
extern char pages[MAX_PAGES];

void mm_init();
unsigned long alloc_page();
void free_page(unsigned long addr);
void* malloc(int size);
void free(void* obj);
void map_page(unsigned long pml4_pa, unsigned long from_va, unsigned long to_pa, char us);
