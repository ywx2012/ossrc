#pragma once

#include <stdint.h>

struct interrupt_frame {
  uintptr_t eip;
  uintptr_t cs;
  uintptr_t eflags;
  uintptr_t esp;
  uintptr_t ss;
};

typedef void (*int_handler)(struct interrupt_frame *);
typedef void (*exc_handler)(struct interrupt_frame *, uintptr_t);

void idt_set_int_handler(uint8_t number, int_handler);
void idt_set_exc_handler(uint8_t number, exc_handler);
