
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include "include/segment.h"

.text
.code64
.globl gdt
.globl ret_from_kernel
.globl task0_stack
.globl idle_task_entry
start64:
  lgdt gdtr
  lidt idtr

  // setup seg 
  mov $KERNEL_DS, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs
  mov %ax, %ss

  // setup stack
  mov $task0_stack, %rsp

  pushq $main
  ret

gdt:
  .quad 0x0000000000000000  /* reserved */
  .quad 0x00209a0000000000  /* kernel cs */
  .quad 0x0000920000000000  /* kernel ds */
  .quad 0x0000000000000000  /* user32 cs */
  .quad 0x0000f20000000000  /* user ds */
  .quad 0x0020fa0000000000  /* user64 cs */
  .fill 64 - 6, 8, 0
gdt_end:

gdtr:
  .word gdt_end - gdt
  .quad gdt

idtr:
  .word 16 * 256
  .quad idt_table

// 4kb
.fill 4096, 1, 0
task0_stack:

ret_from_kernel:
  mov $USER_DS, %rax
  movw %ax, %ds
  movw %ax, %es
  movw %ax, %fs
  movw %ax, %gs
  iretq

idle_task_entry:
1:
  sti
  hlt
  jmp 1b
