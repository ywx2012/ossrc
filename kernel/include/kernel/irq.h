#pragma once

#include <x86/idt.h>

void irq_set_handler(uint8_t number, uint8_t ist, int_handler);
