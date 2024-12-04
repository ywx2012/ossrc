#pragma once

#include <stdint.h>
#include <stddef.h>
#include <list.h>

struct task {
  uintptr_t id;
  uintptr_t *pml4;
  uintptr_t rsp0;
  uintptr_t jmp_buf[5];
  struct node task_node;
  uintptr_t wakeup;
  struct node timer_node;
};

uintptr_t task_create(size_t size, char const *data);
struct task *task_dequeue(void);
void task_enqueue(struct task *);
void task_yield(void);
void task_switch(void);

int system_call(uintptr_t rdi, uintptr_t rsi, uintptr_t rdx);

extern struct task *current;
extern struct task idle;
