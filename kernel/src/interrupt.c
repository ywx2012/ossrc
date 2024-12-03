
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <stdint.h>
#include <sys/io.h>
#include <string.h>
#include <interrupt.h>
#include <segment.h>
#include <x86/pic.h>
#include <x86/segment.h>

#define IRQ_OFFSET 0x20
#define COUNTER (1193181 / 100)
#define GATE_INTERRUPT 0xe
#define GATE_EXCEPTION 0xf

struct gate_desc {
  uint16_t offset_low;
  uint16_t segment;
  uint16_t ist : 3, zero : 5, type : 4, zero2 : 1, dpl : 2, p : 1;
  uint16_t offset_middle;
  uint32_t offset_high;
  uint32_t reserved;
} __attribute__((packed));

struct gate_desc idt_table[256];

void init_8254() {
  outb(0x36, 0x43);
  outb((COUNTER&0xff), 0x40);
  outb((COUNTER>>8), 0x40);
}

static void set_gate(unsigned char index, unsigned long addr, char type) {
  struct gate_desc* desc = &idt_table[index];

  memset(desc, 0, sizeof(struct gate_desc));
  desc->segment = KERNEL_CS;
  desc->offset_low = (uint16_t)addr;
  desc->offset_middle = (uint16_t)(addr >> 16);
  desc->offset_high = (uint32_t)(addr >> 32);
  desc->dpl = 0;
  desc->type = type;
  desc->p = 1;
}

void interrupt_init() {
  set_gate(14, (unsigned long)&pf_handler, GATE_EXCEPTION);
  set_gate(IRQ_OFFSET+IRQ_PIT0, (unsigned long)&timer_handler, GATE_INTERRUPT);
  set_gate(IRQ_OFFSET+IRQ_KBD, (unsigned long)&kb_handler, GATE_INTERRUPT);

  static struct dtr IDTR __attribute__((aligned(16))) = {
    .base = (uintptr_t)idt_table,
    .limit = sizeof(idt_table) - 1,
  };
  __asm__("lidt %0" : : "m"(IDTR));

  pic_init(IRQ_OFFSET, IRQ_OFFSET+0x8);
  pic_enable(IRQ_PIT0);
  pic_enable(IRQ_KBD);
}
