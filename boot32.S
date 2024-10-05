
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

# Setup kmap level 2
  mov $0x112000, %edi
  mov $0x113000 + 3, %eax
1:
  stosl
  add $0x1000, %eax
  add $4, %edi

  cmp $(0x112000 + 32 * 8 - 8), %edi
  jle 1b

# Fill pt
  mov $0x113000, %edi
  mov $0x0 + 3, %eax
2:
  stosl
  add $0x1000, %eax
  add $4, %edi

  cmp $(0x113000 + 512 * 32 * 8 - 8), %edi
  jle 2b

# Setup direct-map level 2
  mov $0x136000, %edi
  mov $0x113000 + 3, %eax
3:
  stosl
  add $0x1000, %eax
  add $4, %edi

  cmp $(0x136000 + 32 * 8 - 8), %edi
  jle 3b

  # Enable PAE
  xorl %eax, %eax
  btsl $5, %eax
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
  .quad 0x0000000000135003
  .fill 237, 8, 0
  .quad 0x0000000000111003

.org 0x11000
pml3:
  .fill 510, 8, 0
  .quad 0x0000000000112003
  .fill 1, 8, 0

.org 0x12000
pml2:
  .fill 512, 8, 0

.org 0x13000
#pml1
pt:
  .fill 512 * 32, 8, 0

# id map
.org 0x33000
  .quad 0x00000000000112003
  .fill 511, 8, 0

// .org 0x34000
//  .quad 0x0000000000113003
//  .fill 511, 8, 0

## linear map
# pml3
.org 0x35000
  .quad 0x0000000000136003
  .fill 511, 8, 0

# pml2
.org 0x36000
  .fill 32, 8, 0
  .quad 0x0000000000137003
  .fill 479, 8, 0

# pml1
.org 0x37000
  .fill 512, 8, 0
