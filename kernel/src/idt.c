#include <x86/segment.h>
#include <x86/idt.h>
#include <bsp.h>
#include <selector.h>

static struct gate idt[256] __attribute__((aligned(PAGE_SIZE))) = {0};
static struct dtr idtr __attribute__((aligned(16))) = {
  .base = (uintptr_t)idt,
  .limit = sizeof(idt) - 1,
};

static
void
set_gate(uint8_t number, uint8_t type, uint8_t ist, uintptr_t offset) {
  struct gate gate = {0};
  gate.s = 0;
  gate.size = 1;
  gate.type = type&0x7;
  gate.dpl = 0;
  gate.segment_selector = KERNEL_CS;
  gate.offset = (offset & 0xFFFF);
  gate.offset_l = (offset >> 16) & 0xFFFF;
  gate.offset_q = (uint32_t)((offset >> 32) & 0xFFFFFFFF);
  gate.present = 1;
  gate.ist = ist&0x7;
  idt[number] = gate;
}

void
idt_init(void) {
  __asm__("lidt %0" : : "m"(idtr));
}

void
idt_set_int_handler(uint8_t number, uint8_t ist, void (*handler)(struct interrupt_frame *)) {
  set_gate(number, GATE_TYPE_INT, ist, (uintptr_t)handler);
}

void
idt_set_exc_handler(uint8_t number, uint8_t ist, void (*handler)(struct interrupt_frame *, uintptr_t)) {
  set_gate(number, GATE_TYPE_TRAP, ist, (uintptr_t)handler);
}
