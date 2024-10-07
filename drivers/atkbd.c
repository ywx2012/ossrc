
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <sys/io.h>
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

#define DATA_REG 0x60
#define CMD_REG 0x64

#define ENABLE_PORT1 0xAE
#define DISABLE_PORT1 0xAD
#define DISABLE_PORT2 0xA7
#define READ_BYTE 0x20
#define WRITE_BYTE 0x60
#define TRANSLATION_FLAG 0x40

#define SET_SCANCODESET 0xF0

void atkbd_init() {
  outb(DISABLE_PORT1, CMD_REG);
  outb(DISABLE_PORT2, CMD_REG);

  while(inb(CMD_REG) & 0x1)
    inb(DATA_REG);

  outb(READ_BYTE+0, CMD_REG);
  unsigned char c = inb(DATA_REG);
  outb(WRITE_BYTE+0, CMD_REG);
  outb(c^(c&TRANSLATION_FLAG),  DATA_REG);

  outb(SET_SCANCODESET, DATA_REG);
  outb(0x02, DATA_REG);

  outb(ENABLE_PORT1, CMD_REG);
}
