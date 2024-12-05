SECTIONS {
    . = 0x10000;
    __setup_start = .;
    .setup : AT(0x0) {
      build/kernel/header.o(.text);
      build/kernel/realmode.o(.text);
      build/kernel/setup.o(.text .rodata .rodata.* .data .bss);
      FILL(0);
      . = ALIGN(4096);
    }
    __setup_end = .;
    __setup_size = SIZEOF(.setup);
    __setup_sects = ABSOLUTE((__setup_size>>9)-1);

    . = 0xFFFF800000100000;
    __kernel_start = .;
    .data : AT(ADDR(.data) - __kernel_start + SIZEOF(.setup)) {
      build/kernel/header.o(.data);
      *(.data .rodata .rodata.* .ldata);
      *(.bss);
    }
    .text : AT(ADDR(.text) - __kernel_start + SIZEOF(.setup)) {
      *(.text);
      FILL(0);
      . = ALIGN(4096);
    }
    __kernel_end = .;

    /DISCARD/ : {
      *(.note.*);
      *(.eh_frame);
    }
}
