
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <task.h>
#include <interrupt.h>
#include <paging.h>

__attribute__((interrupt))
void
pf_handler(struct interrupt_frame *frame, uintptr_t error_code) {
  uintptr_t addr;
  __asm__("mov %%cr2, %0" : "=r"(addr));
  paging_alloc_page(current->pml4, addr, PTE_W|PTE_U);
}
