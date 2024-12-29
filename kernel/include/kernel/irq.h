#pragma once

#include <x86/idt.h>

void irq_set_handler(uint8_t number, int_handler);
