SECTIONS {
    . = 0x100000;
    .text : {
      *(.start);
      *(.text);
      . = ALIGN(0x1000);
    }

    .rodata : {
      *(.rodata .rodata.*);
      . = ALIGN(0x1000);
    }

    .data : {
      *(.data .data.*);
      . = ALIGN(0x1000);
    }

    .bss : {
      *(.bss .bss.*);
      . = ALIGN(0x1000);
    }

    /DISCARD/ : {
      *(.note.*)
    }
}
