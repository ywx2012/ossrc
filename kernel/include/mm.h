
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#pragma once

#include <x86/page.h>

#define PAGE_MASK 0xffffffffff000

unsigned long alloc_page();
void map_page(unsigned long pml4_pa, unsigned long from_va, unsigned long to_pa, char us);
