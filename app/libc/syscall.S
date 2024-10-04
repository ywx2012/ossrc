
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#define SYSCALL_SLEEP       0
#define SYSCALL_SHM         1
#define SYSCALL_FBMAP       2
#define SYSCALL_GETMODEINFO 3

.globl sleep
.globl shm_open
.globl fbmap
.globl get_mode_info

sleep:
  mov $SYSCALL_SLEEP, %rax
  syscall
  ret

shm_open:
  mov $SYSCALL_SHM, %rax
  syscall
  ret

fbmap:
  mov $SYSCALL_FBMAP, %rax
  syscall
  ret

get_mode_info:
  mov $SYSCALL_GETMODEINFO, %rax
  syscall
  ret
