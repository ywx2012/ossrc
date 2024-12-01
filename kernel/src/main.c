
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <mm.h>
#include <print.h>
#include <sched.h>
#include <tss.h>
#include <segment.h>
#include <interrupt.h>
#include <syscall.h>
#include <vesa.h>
#include <atkbd.h>

int main() {
  mm_init();
  interrupt_init();
  syscall_init(); 
  vesa_init();
  atkbd_init();

  sched_init();
  tss_init();
  init_8254();

  __asm__ ("mov %0, %%cr3": :"r"(current->pml4));
  __asm__ ("mov %0, %%rsp": :"m"(current->rsp0));

  __asm__ ("sti");

  __asm__ ("jmp ret_from_kernel");
}
