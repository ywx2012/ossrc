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
struct task *task_dequeue();
void task_enqueue(struct task *);
void task_yield();
void task_resume();

extern struct task *current;
extern struct task idle;
