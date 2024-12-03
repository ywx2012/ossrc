OUTPUT_FORMAT(binary);

SECTIONS {
    . = 0x10000;
    .setup : AT(0x0) {
      build/kernel/header.o(.text);
      build/kernel/realmode.o(.text);
      build/kernel/setup.o(.text .rodata .rodata.* .data .bss);
      FILL(0);
      . = ALIGN(512);
    }
    __setup_size = SIZEOF(.setup);
    __setup_sects = ABSOLUTE((__setup_size>>9)-1);

    . = 0xFFFF800000100000;
    __kernel_start = .;
    .text : AT(ADDR(.text) - __kernel_start + SIZEOF(.setup)) {
      build/kernel/header.o(.data);
      *(.text);
    }
    .data : AT(ADDR(.data) - __kernel_start + SIZEOF(.setup)) {
      *(.data .rodata .rodata.*);
      *(.bss);
    }
    __kernel_end = .;

    /DISCARD/ : {
      *(.note.*);
      *(.eh_frame);
    }
}
