#include <x86/pic.h>
#include <x86/idt.h>
#include <kernel/irq.h>
#include <kernel/bsp.h>

#define IRQ_OFFSET 0x20

void
irq_init(void) {
  pic_init(IRQ_OFFSET, IRQ_OFFSET+0x8);
}

void
irq_set_handler(uint8_t number, int_handler handler) {
  idt_set_int_handler((uint8_t)(IRQ_OFFSET+number), handler);
}
