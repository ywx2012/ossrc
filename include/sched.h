
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#pragma once

#include <stdint.h>

enum task_state {
  TASK_RUNNING = 0,
  TASK_INTERRUPTIBLE
};

struct task {
  unsigned long id;
  enum task_state state;
  unsigned long rip;
  unsigned long rsp0;
  unsigned long kstack;
  unsigned long pml4;

  struct task* next;
  struct task* prev;
};

struct timer {
  unsigned long alarm;
  struct task* task;
  struct timer* next;
  struct timer* prev;
};

extern unsigned long ret_from_kernel;
extern unsigned long idle_task_entry;
extern unsigned long task0_stack;
extern struct task* current;

void sched_init();
