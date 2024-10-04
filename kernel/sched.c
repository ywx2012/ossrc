
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include "include/mm.h"
#include "include/sched.h"
#include "include/string.h"
#include "include/segment.h"
#include "include/print.h"
#include "include/tss.h"

static struct task* task_head;
static struct task* task_tail;
struct task* current;
struct task* idle_task;

unsigned long ticks;
struct timer* timer_head;
struct timer* timer_tail;

static void fake_task_stack(unsigned long kstack) {
  uint16_t ss = USER_DS;
  unsigned long rsp = 0x8000000;
  uint16_t cs = USER_CS;
  unsigned long rip = 0x100000;

  unsigned long rsp_tmp;
  __asm__ ("mov %%rsp, %5\n\t"
           "mov %4, %%rsp\n\t"
           "pushq %0\n\t"
           "pushq %1\n\t"
           "pushf\n\t"
           "pushq %2\n\t"
           "pushq %3\n\t"
           "mov %5, %%rsp\n\t"
           :
           : "m"(ss), "m"(rsp), "m"(cs), "m"(rip), "m"(kstack), "m"(rsp_tmp));
}

static void make_task(unsigned long id, unsigned long entry, unsigned long entry_pa) {
  struct task* task = malloc(sizeof(struct task));
  task->id = id;
  task->state = TASK_RUNNING;

  task->pml4 = alloc_page();
  memset(VA(task->pml4), 0, PAGE_SIZE);

  memcpy(VA(task->pml4 + 8 * 256), VA(TASK0_PML4 + 8 * 256), 8 * 256);
  map_range(task->pml4, entry, entry_pa, 0x4, 1024);

  task->kstack = (unsigned long)VA(alloc_page()) + PAGE_SIZE;
  
  fake_task_stack(task->kstack);
  task->rsp0 = task->kstack - 8 * 5;
  task->rip = (unsigned long)&ret_from_kernel;

  if (!task_head) {
    task_head = task;
    task_tail = task;
    task->prev = NULL;
    task->next = NULL;
  } else {
    task_tail->next = task;
    task->prev = task_tail;
    task->next = NULL;
    task_tail = task;
  }
}

static void make_idle_task() {
  idle_task = malloc(sizeof(struct task));
  idle_task->id = 0;
  idle_task->state = TASK_RUNNING;
  idle_task->pml4 = TASK0_PML4;
  idle_task->kstack = (unsigned long)&task0_stack;
  idle_task->rsp0 = (unsigned long)&task0_stack;
  idle_task->rip = (unsigned long)&idle_task_entry;
}

void sched_init() {
  make_task(1, 0x100000, 0xc800000);
  make_task(2, 0x100000, 0xd000000);  
  make_idle_task();

  current = task_head; 
}

void schedule() {
  struct task* next = NULL;

  for (struct task* t = task_head; t; t = t->next) {
    if (t->state == TASK_RUNNING) {
      next = t;
      break;
    }
  }

  if (!next) {
    next = idle_task;
  }

  if (next != current) {
    __asm__ ("mov %%rsp, %0\n\t" /* save rsp */          \
           "movq $1f, %1\n\t"  /* save rip */          \
           "mov %2, %%rsp\n\t" /* restore rsp */       \
           "push %3\n\t"       /* restore rip */       \
           : "=m"(current->rsp0), "=m"(current->rip)    \
           : "m"(next->rsp0), "m"(next->rip)  \
    );

    // switch kstack
    tss.rsp0 = (unsigned long)next->kstack;

    // update current
    current = next;

    // switch cr3
    __asm__ ("mov %0, %%cr3" : : "a" (next->pml4));
    // trigger
    __asm__ ("ret");

    __asm__ ("1:");
  }
}

void do_timer() {
  ticks++;

  // check timer
  for (struct timer* t = timer_head; t; t = t->next) {
    if (t->alarm <= ticks) {
      t->task->state = TASK_RUNNING;

      if (t == timer_head && t == timer_tail) {
        timer_head = NULL;
        timer_tail = NULL;
      } else if (t == timer_head) {
        timer_head = t->next;
        t->next->prev = NULL;
      } else if (t == timer_tail) {
        timer_tail = t->prev;
        t->prev->next = NULL;
      } else {
        t->prev->next = t->next;
        t->next->prev = t->prev;
      }

      free(t);
    }
  }

  if (current != idle_task) {
    if (current != task_tail) {
      if (current->prev) {
        current->prev->next = current->next;
      }
      current->next->prev = current->prev;

      current->prev = task_tail;
      task_tail->next = current;

      if (current == task_head) {
        task_head = current->next;
      }
      task_tail = current;

      current->next = NULL;
    }
  }

  schedule();
}

int do_sleep(long ms) {
  struct timer* t = malloc(sizeof(struct timer));
  t->task = current;
  t->alarm = ticks + ms / 10;

  if (!timer_head) {
    timer_head = t;
    timer_tail = t;
    t->prev = t->next = NULL;
  } else {
    timer_tail->next = t;
    t->prev = timer_tail;
    t->next = NULL;
    timer_tail = t;
  }

  current->state = TASK_INTERRUPTIBLE;
  schedule();

  return 0;
}
