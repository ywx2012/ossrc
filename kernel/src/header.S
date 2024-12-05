#include <asm/boot.h>
#include <asm/bootparam.h>
#include <x86/page.h>
#include <setup/header.h>

#define PA_FROM_VA(x) (((x)-code32_start)+CODE32_START)

        .code16
        .globl setup_header
        .text
base:
        // https://www.kernel.org/doc/html/latest/arch/x86/boot.html
        .fill 0x1F1, 1, 0
setup_header:
        // setup_sects:
        .byte   __setup_sects
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
        .word   0x1000
        // kernel_version
        .word   0
        // type_of_loader
        .byte   0
        // loadflags
        .byte   LOADED_HIGH
        // setup_move_size
        .word   0x8000
        // code32_start
        .long   CODE32_START
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
exit:
        ljmpl   $(KERNEL_CS),   $setup_start
start:
        cli
        // disable NMI
        mov     $0x80,  %ax
        mov     $0x70,  %dx
        out     %ax,    %dx

        pushl   $PA_FROM_VA(pml4)
        pushl   $(gdtr-base)
        pushl   $(e820map-base)

        // workaround ld: final link failed
        call    1f
1:
        xor     %eax, %eax
        pop     %ax
        subl    $(1b-exit), %eax
        pushl   %eax

next:
        nop

        .data
        .align  0x1000
code32_start:
pml3:
        .quad   PTE_P|PTE_W|PTE_PS
        .fill   511, 8, 0

        .align  0x1000
        .globl  pml4
pml4:
        .quad   PA_FROM_VA(pml3)|PTE_P|PTE_W
        .fill   255, 8, 0
        .quad   PA_FROM_VA(pml3)|PTE_P|PTE_W
        .fill   255, 8, 0
