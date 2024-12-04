
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <stdint.h>
#include <sys/io.h>
#include <string.h>
#include <interrupt.h>
#include <selector.h>
#include <x86/pic.h>
#include <x86/segment.h>

#define IRQ_OFFSET 0x20
#define COUNTER (1193181 / 100)
#define GATE_INTERRUPT 0xe
#define GATE_EXCEPTION 0xf

void init_8254() {
  outb(0x36, 0x43);
  outb((COUNTER&0xff), 0x40);
  outb((COUNTER>>8), 0x40);
}

