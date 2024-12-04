#pragma once

#include <stdint.h>

struct interrupt_frame {
  uintptr_t rip;
  uintptr_t cs;
  uintptr_t rflags;
  uintptr_t rsp;
  uintptr_t ss;
};

typedef void (*int_handler)(struct interrupt_frame *);
typedef void (*exc_handler)(struct interrupt_frame *, uintptr_t);

void idt_set_int_handler(uint8_t number, uint8_t ist, int_handler);
void idt_set_exc_handler(uint8_t number, uint8_t ist, exc_handler);
