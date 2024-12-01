
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <segment.h>

.text
.code64
.globl ret_from_kernel
.globl task0_stack
.globl idle_task_entry

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
