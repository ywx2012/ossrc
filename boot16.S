
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#define __ASSEMBLY__
#include <asm/boot.h>
#include <asm/bootparam.h>

#define IO_PIC     0x20
#define IRQ_OFFSET 32

.text
.code16
        // https://www.kernel.org/doc/html/latest/arch/x86/boot.html
        .fill 0x1F1, 1, 0
        // setup_sects:
        .byte   0x20
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
  mov $e820_entry, %di
  xor %ebx, %ebx
e820_rd_entry:
  mov $0xe820, %eax
  mov $20, %ecx
  int $0x15

  add $20, %di
  incb e820_nr_entry

  cmp $0, %ebx
  jne e820_rd_entry

  # get display mode info
  mov $vesa_mode_info, %di
  mov $0x4f01, %ax
  int $0x10

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

  lgdt gdtr

  mov %cr0, %eax
  or $0x1, %eax
  mov %eax, %cr0

  ljmpl $0x8, $0x100000

gdtr:
  .word gdt_end - gdt
  .word gdt, 1

gdt:
  .quad 0x0000000000000000
  .quad 0x00c09a00000007ff
  .quad 0x00c09200000007ff
gdt_end:

# e820map
.org 0x3000
e820_nr_entry:
  .long 0
e820_entry:
  .fill 512, 1, 0

# vesa mode info
.org 0x4000
vesa_mode_info:
  .fill 256, 1, 0

  .align 512, 0
  .org 0x4200
