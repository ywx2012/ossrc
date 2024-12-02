#include <sys/io.h>
#include <x86/pic.h>

#define A0 1
#define ICW1 0x10
#define ICW1_IC4 0x1
#define ICW4_8086 0x1

#define OCW2 0
#define OCW2_SL 0x40
#define OCW2_EOI 0x20
#define OCW3 0x08
#define OCW3_RR 0x02
#define OCW3_RIS 0x01

#define IOPORT_PIC0 0x20
#define IOPORT_PIC1 0xA0

static
inline
unsigned char
pic_get_mask(unsigned char port) {
  return inb(port|A0);
}

static
inline
void
pic_set_mask(unsigned char mask, unsigned char port) {
  outb(mask, port|A0);
}

static
inline
unsigned char
pic_get_isr(unsigned char port) {
  outb(OCW3|OCW3_RR|OCW3_RIS, port);
  return inb(port);
}

static
inline
void
pic_eoi(unsigned char port, unsigned char irq) {
  outb(OCW2|OCW2_SL|OCW2_EOI|irq, port);
}

void
pic_init(unsigned char offset1, unsigned char offset2) {
  outb(ICW1|ICW1_IC4, IOPORT_PIC0);
  outb(ICW1|ICW1_IC4, IOPORT_PIC1);
  // ICW2
  outb(offset1, IOPORT_PIC0|A0);
  outb(offset2, IOPORT_PIC1|A0);
  // ICW3
  outb(1<<IRQ_PIC1, IOPORT_PIC0|A0);
  outb(IRQ_PIC1, IOPORT_PIC1|A0);
  // ICW4
  outb(ICW4_8086, IOPORT_PIC0|A0);
  outb(ICW4_8086, IOPORT_PIC1|A0);

  pic_set_mask(0xff, IOPORT_PIC0);
  pic_set_mask(0xff, IOPORT_PIC1);
  pic_enable(IRQ_PIC1);
}

void
pic_enable(unsigned char irq) {
  unsigned port = (irq<8)?IOPORT_PIC0:IOPORT_PIC1;
  unsigned char mask = pic_get_mask(port);
  mask ^= mask & (unsigned char)(1 << (irq % 8));
  pic_set_mask(mask, port);
}

void
pic_disable(unsigned char irq) {
  unsigned port = (irq<8)?IOPORT_PIC0:IOPORT_PIC1;
  unsigned char mask = pic_get_mask(port);
  mask |= (unsigned char)(1 << (irq % 8));
  pic_set_mask(mask, port);
}

bool
pic_is_spurious(unsigned char irq) {
  unsigned port = (irq<8)?IOPORT_PIC0:IOPORT_PIC1;
  unsigned char isr = pic_get_isr(IOPORT_PIC0);
  unsigned char mask = (unsigned char)(1 << (irq % 8));
  if (isr & mask)
    return false;
  return true;
}

void
pic_acknowledge(unsigned char irq, bool is_spurious) {
  unsigned port = (irq<8)?IOPORT_PIC0:IOPORT_PIC1;
  if (!is_spurious)
    pic_eoi(irq % 8, port);
  if (port == IOPORT_PIC1)
    pic_eoi(IRQ_PIC1, IOPORT_PIC0);
}
