OUTPUT_FORMAT(binary);

SECTIONS {
    . = 0xffff8000000ff000;
    .text : {
      build/kernel/boot16.o(.text);
      *(.text);
    }

    /DISCARD/ : {
      *(.note.*)
    }
}
