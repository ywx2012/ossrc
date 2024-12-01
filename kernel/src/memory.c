
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <string.h>
#include <print.h>
#include <mm.h>
#include <sched.h>
#include <interrupt.h>

char pages[MAX_PAGES];
unsigned long mem_size = 0;

void map_page(unsigned long pml4_pa, unsigned long from_va, unsigned long to_pa, char us) {
  unsigned long *entry = &pml4_pa;
  char offsets[3] = {39, 30, 21};

  for (int i=0; i<3; ++i) {
    short index = (from_va >> offsets[i]) & 0x1ff;
    entry = ((unsigned long *)VA((*entry) & PAGE_MASK)) + index;
    if (!((*entry) & 0x1))
      *entry = alloc_page() | 0x3 | us;
  }

  // pml1
  short index = (from_va >> 12) & 0x1ff;
  entry = ((unsigned long *)VA((*entry) & PAGE_MASK)) + index;
  if (!((*entry) & 0x1))
    *entry = to_pa | 0x3 | us;
}

void do_page_fault(unsigned long addr) {
  unsigned long pa = alloc_page();
  map_page(current->pml4, addr, pa, 0x4);
}

__attribute__((interrupt))
void
pf_handler(struct interrupt_frame *frame, uintptr_t error_code) {
  uintptr_t cr2;
  __asm__("mov %%cr2, %0" : "=r"(cr2));
  do_page_fault(cr2);
}

void mm_init() {
  for (int i = 0; i < e820->nr_entry; i++) {
    if (e820->map[i].type == E820_RAM) {
      unsigned long tmp = e820->map[i].addr + e820->map[i].size;
      if (tmp > mem_size) {
        mem_size = tmp;
      }
    }
  }

  memset(pages, 0, MAX_PAGES);

  for (int i = 0; i < KERNEL_PAGE_NUM; i++) {
    pages[i] = 1;
  }
}
