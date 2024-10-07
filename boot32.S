
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

.text
.code32
start32:
  mov $0x10, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs
  mov %ax, %ss

  lgdt gdt

  xorl %eax, %eax
  # Enable PAE
  btsl $5, %eax
  # Enable PSE
  btsl $4, %eax
  movl %eax, %cr4

  # Setup cr3
  movl  $0x110000, %eax
  movl  %eax, %cr3

  # Setup EFER
  movl  $0xc0000080, %ecx
  rdmsr
  # Enable Long Mode
  btsl  $8, %eax
  # Enable syscall/sysret
  btsl  $0, %eax
  wrmsr

  # Activate long mode
  xorl %eax, %eax
  # Enable paging
  btsl $31, %eax
  # Enable protected mode
  btsl $0, %eax
  movl %eax, %cr0
 
  ljmp $0x8, $0x200000

gdt_start:
  .quad   0x0000000000000000
  .quad   0x00a09a0000000000
  .quad   0x00c0920000000000 
gdt_end:

gdt:
  .word gdt_end - gdt_start
  .quad gdt_start

.org 0x10000
pml4:
  .quad 0x0000000000133003
  .fill 272, 8, 0
  .quad 0x0000000000133003
  .fill 237, 8, 0
  .quad 0x0000000000111003

.org 0x11000
pml3:
  .fill 510, 8, 0
  .quad 0x0000000000000083
  .fill 1, 8, 0

# id map
.org 0x33000
  .quad 0x0000000000000083
  .fill 511, 8, 0
