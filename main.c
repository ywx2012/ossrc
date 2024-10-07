
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include "include/mm.h"
#include "include/print.h"
#include "include/sched.h"
#include "include/tss.h"
#include "include/segment.h"
#include "include/interrupt.h"
#include "include/syscall.h"
#include "include/vesa.h"
#include "include/atkbd.h"

int main() {
  mm_init();
  interrupt_init();
  syscall_init(); 
  vesa_init();
  atkbd_init();

  // __asm__ ("sti");
  sched_init();
  tss_init();

  __asm__ ("mov %0, %%cr3": :"r"(current->pml4));

  init_8254();

  __asm__ ("movq %0, %%rsp\n\t"
           "jmp ret_from_kernel\n\t"
           :
           : "m"(current->rsp0)
          );
}
