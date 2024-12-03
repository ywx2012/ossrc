
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#pragma once

#include <stdint.h>
#include <list.h>

enum task_state {
  TASK_RUNNING = 0,
  TASK_INTERRUPTIBLE
};

struct task {
  unsigned long id;
  enum task_state state;
  uintptr_t reserved[2];
  unsigned long kstack;
  uintptr_t *pml4;
  uintptr_t jmp_buf[5];

  struct node task_node;

  unsigned long alarm;
  struct node timer_node;
};

extern unsigned long ret_from_kernel;
extern unsigned long idle_task_entry;
extern unsigned long task0_stack;
extern struct task* current;

void sched_init();
void resume_task();
