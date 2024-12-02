#pragma once

#include <stdbool.h>

#define IRQ_PIT0 0
#define IRQ_KBD  1
#define IRQ_PIC1 2

void pic_init(unsigned char offset1, unsigned char offset2);
void pic_enable(unsigned char irq);
void pic_disable(unsigned char irq);
bool pic_is_spurious(unsigned char irq);
void pic_acknowledge(unsigned char irq, bool is_spurious);
