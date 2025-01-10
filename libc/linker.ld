SECTIONS {
    . = 0x100000;
    .text : {
      *(.start);
      *(.text);
    }

    /DISCARD/ : {
      *(.note.*)
    }
}
