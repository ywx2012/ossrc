#include <x86/pic.h>
#include <x86/idt.h>
#include <irq.h>

#define IRQ_OFFSET 0x20

void
irq_init(void) {
  pic_init(IRQ_OFFSET, IRQ_OFFSET+0x8);
}

void
irq_set_handler(uint8_t number, uint8_t ist, int_handler handler) {
  idt_set_int_handler(IRQ_OFFSET+number, ist, handler);
}
