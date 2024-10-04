
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include "include/types.h"
#include "include/print.h"

// scancode set 2
unsigned char keymap[256] = {
  [0x1c] = 'a',
  [0x32] = 'b',
  [0x21] = 'c',
  [0x23] = 'd',
  [0x24] = 'e',
  [0x2b] = 'f',
  [0x34] = 'g',
  [0x33] = 'h',
  [0x43] = 'i',
  [0x3b] = 'j',
  [0x42] = 'k',
  [0x4b] = 'l',
  [0x3a] = 'm',
  [0x31] = 'n',
  [0x44] = 'o',
  [0x4d] = 'p',
  [0x15] = 'q',
  [0x2d] = 'r',
  [0x1b] = 's',
  [0x2c] = 't',
  [0x3c] = 'u',
  [0x2a] = 'v',
  [0x1d] = 'w',
  [0x22] = 'x',
  [0x35] = 'y',
  [0x1a] = 'z',
  [0x45] = '0',
  [0x16] = '1',
  [0x1e] = '2',
  [0x26] = '3',
  [0x25] = '4',
  [0x2e] = '5',
  [0x36] = '6',
  [0x3d] = '7',
  [0x3e] = '8',
  [0x46] = '9'
};

void process_kb() {
  unsigned char scancode;
  static unsigned char prevcode;

  __asm__ ("inb $0x60, %%al" : "=a"(scancode));
	
  if (scancode == 0xe0 || scancode == 0xf0) {
    prevcode = scancode;
    return;
  }

  if (prevcode == 0xe0 || prevcode == 0xf0) {
    prevcode = 0;
    return;
  }
	
  print(keymap[scancode]);	
}
