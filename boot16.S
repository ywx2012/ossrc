
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#define __ASSEMBLY__
#include <asm/boot.h>
#include <asm/bootparam.h>

#include "include/segment.h"

#define IO_PIC     0x20
#define IRQ_OFFSET 32
#define PA(x) ((x)-start64+0x100000)

.text
.code16
        // https://www.kernel.org/doc/html/latest/arch/x86/boot.html
        .fill 0x1F1, 1, 0
        // setup_sects:
        .byte   0x7
        // root_flags
        .word   0
        // syssize
        .long   0
        // ram_size
        .word   0
        // vid_mode
        .word   ASK_VGA
        // root_dev
        .word   0
        // boot_flags
        .word   0xAA55

_start:
        jmp  start
1:
        // make sure jmp is 2 bytes
        // .byte 0
        .org 0x202
        // header
        .ascii  "HdrS"
        // version
        .word   0x020f
        // realmode_swtch
        .long   0
        // start_sys_seg
        .word   0
        // kernel_version
        .word   kernel_version - _start
        // type_of_loader
        .byte   0
        // loadflags
        .byte   LOADED_HIGH
        // setup_move_size
        .word   0x8000
        // code32_start
        .long   0x100000
        // ramdisk_image
        .long   0
        // ramdisk_size
        .long   0
        // bootsect_kludge
        .long   0
        // heap_end_ptr
        .word   0
        // ext_loader_ver
        .byte   0
        // ext_loader_type
        .byte   0
        // cmd_line_ptr
        .long   0
        // initrd_addr_max
        .long   0x7ffffff
        // kernel_alignment
        .long   0x1000
        // relocatable_kernel
        .byte   0
        // min_alignment
        .byte   21
        // xloadflags
        .word   XLF_KERNEL_64
        // cmdline_size
        .long   0
        // hardware_subarch
        .long   0
        // hardware_subarch_data
        .quad   0
        // payload_offset
        .long   0
        // payload_length
        .long   0
        // setup_data
        .quad   0
        // pref_address
        .quad   0
        // init_size
        .long   0
        // handover_offset
        .long   0
        // kernel_info_offset
        .long   0

kernel_version:
        .asciz  "I am not Linux"

start:
  cli

  # read e820
  mov $0x0534D4150, %edx
  mov $(e820_entry-_start+0x200), %di
  xor %ebx, %ebx
e820_rd_entry:
  mov $0xe820, %eax
  mov $20, %ecx
  int $0x15

  add $20, %di
  incb (e820_nr_entry-_start+0x200)

  cmp $0, %ebx
  jne e820_rd_entry

  # Init 8259A
  # ICW1
  mov $0x13, %al
  mov $(IO_PIC), %dx
  out %al,%dx
  # ICW2
  mov $(IRQ_OFFSET), %al
  mov $(IO_PIC+1), %dx
  out %al, %dx
  # ICW4
  mov $0x1, %al
  mov $(IO_PIC+1), %dx
  out %al, %dx

  lgdt (gdtr16-_start+0x200)

  xorl %eax, %eax
  # Enable PAE
  btsl $5, %eax
  # Enable PSE
  btsl $4, %eax
  movl %eax, %cr4

  # Setup cr3
  movl  $PA(pml4), %eax
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

  ljmpl $0x8, $0x100000

gdtr16:
  .word gdt_end - gdt
  .quad gdt

e820_nr_entry:
  .long 0
e820_entry:
  .fill 640, 1, 0

  .org 0x1000

.code64
.globl gdt
start64:
  movabs $gdtr, %rax
  lgdt (%rax)
  movabs $idtr, %rax
  lidt (%rax)

  // setup seg
  mov $KERNEL_DS, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs
  mov %ax, %ss

  // setup stack
  movabs $task0_stack, %rax
  mov %rax, %rsp

  movabs $main, %rax
  pushq %rax
  ret

        .globl e820
e820:
        .quad e820_nr_entry - _start + 0x10200

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

        .align 0x1000, 0
        .globl pml4
pml4:
  .quad PA(pml3)|3
  .fill 255, 8, 0
  .quad PA(pml3)|3
  .fill 255, 8, 0

        .align 0x1000, 0
pml3:
  .quad 0x0000000000000083
  .fill 511, 8, 0
