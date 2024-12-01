OUTPUT_FORMAT(binary);

SECTIONS {
    . = 0x100000;
    .text : {
      build/libc/start.o(.text);
      *(.text);
    }

    /DISCARD/ : {
      *(.note.*)
    }
}
