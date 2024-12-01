
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#pragma once

#include <stdint.h>

enum task_state {
  TASK_RUNNING = 0,
  TASK_INTERRUPTIBLE
};

struct node {
  struct node *prev;
  struct node *next;
};

struct task {
  unsigned long id;
  enum task_state state;
  unsigned long rip;
  unsigned long rsp0;
  unsigned long kstack;
  unsigned long pml4;

  struct node task_node;

  unsigned long alarm;
  struct node timer_node;
};

extern unsigned long ret_from_kernel;
extern unsigned long idle_task_entry;
extern unsigned long task0_stack;
extern struct task* current;

void sched_init();
