#pragma once

#include <stdint.h>
#include <stddef.h>
#include <list.h>

#define KERNEL_CS_INDEX 1
#define KERNEL_SS_INDEX (KERNEL_CS_INDEX+1)
#define USER_CS32_INDEX 3
#define USER_SS_INDEX (USER_CS32_INDEX+1)
#define USER_CS_INDEX (USER_SS_INDEX+1)
#define TSS_INDEX 6
#define KERNEL_CS (KERNEL_CS_INDEX<<3)
#define KERNEL_SS (KERNEL_SS_INDEX<<3)
#define USER_CS32 ((USER_CS32_INDEX<<3)|0x3)
#define USER_CS ((USER_CS_INDEX<<3)|0x3)
#define USER_SS ((USER_SS_INDEX<<3)|0x3)
#define TSS (TSS_INDEX<<3)

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
