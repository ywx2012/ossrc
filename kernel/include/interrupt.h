
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#pragma once

#include <stdint.h>

struct interrupt_frame {
  uintptr_t rip;
  uintptr_t cs;
  uintptr_t rflags;
  uintptr_t rsp;
  uintptr_t ss;
};

void init_8254();
void interrupt_init();

void timer_handler(struct interrupt_frame *frame);
void pf_handler(struct interrupt_frame *frame, uintptr_t error_code);
void kb_handler(struct interrupt_frame *frame);
