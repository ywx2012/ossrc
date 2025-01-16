SECTIONS {
    . = 0x10000;
    __setup_start = .;
    .setup : AT(0x0) {
      *(.header);
      *(.boot.* .setup.*);
      FILL(0);
      . = ALIGN(4096);
    }
    __setup_end = .;
    __setup_size = SIZEOF(.setup);
    __setup_sects = ABSOLUTE((__setup_size>>9)-1);

    . = 0x80100000;
    __kernel_start = .;
    .data : AT(ADDR(.data) - __kernel_start + SIZEOF(.setup)) {
      *(.header.data);
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
