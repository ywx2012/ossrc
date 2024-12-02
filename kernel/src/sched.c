
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <stddef.h>
#include <string.h>
#include <sys/io.h>
#include <mm.h>
#include <sched.h>
#include <segment.h>
#include <print.h>
#include <tss.h>
#include <cpio.h>
#include <interrupt.h>

extern unsigned int *initrd_base;

static struct node task_list;
static struct node timer_list;

struct task* current;
struct task* idle_task;

unsigned long ticks;

static void make_task(unsigned long id, unsigned long entry, char *filename) {
  struct task* task = malloc(sizeof(struct task));
  task->id = id;
  task->state = TASK_RUNNING;

  task->pml4 = alloc_page();
  memset(VA(task->pml4), 0, PAGE_SIZE);

  memcpy(VA(task->pml4 + 8 * 256), VA(TASK0_PML4 + 8 * 256), 8 * 256);

  unsigned long base = *initrd_base;
  struct cpio_newc_header const *cpio = (struct cpio_newc_header const *)base;
  cpio = cpio_lookup(cpio, filename);
  unsigned long filesize = cpio_get_size(cpio);
  char const *p = cpio_get_content(cpio);

  for (unsigned long offset=0; offset<filesize; offset+=PAGE_SIZE) {
    unsigned long entry_pa = alloc_page();
    unsigned long size = filesize - offset;
    if (size > PAGE_SIZE)
      size = PAGE_SIZE;
    memcpy((char *)VA(entry_pa), p+offset, size);
    map_page(task->pml4, entry+offset, entry_pa, 0x4);
  }
  
  task->kstack = (unsigned long)VA(alloc_page()) + PAGE_SIZE;
  
  task->jmp_buf[2] = task->kstack - 8 * 5;
  task->jmp_buf[1] = (unsigned long)&ret_from_kernel;

  list_insert(&task_list, &task->task_node);
}

static void make_idle_task() {
  idle_task = malloc(sizeof(struct task));
  idle_task->id = 0;
  idle_task->state = TASK_RUNNING;
  idle_task->pml4 = TASK0_PML4;
  idle_task->kstack = (unsigned long)&task0_stack;
  idle_task->jmp_buf[2] = (unsigned long)&task0_stack;
  idle_task->jmp_buf[1] = (unsigned long)&idle_task_entry;
}

void sched_init() {
  list_init(&task_list);
  list_init(&timer_list);

  make_task(1, 0x100000, "app1.bin");
  make_task(2, 0x100000, "app2.bin");
  make_idle_task();

  current = STRUCT_FROM_FIELD(struct task, task_node, task_list.next);
}

__attribute__((noipa,naked))
void
resume_task() {
  // switch cr3
  __asm__ ("mov %0, %%cr3" : : "a" (current->pml4));
  // trigger
  __builtin_longjmp(current->jmp_buf, 1);
}

void schedule() {
  struct task* next = idle_task;

  FOREACH(node, task_list) {
    struct task *t = STRUCT_FROM_FIELD(struct task, task_node, node);
    if (t->state == TASK_RUNNING) {
      next = t;
      break;
    }
  }

  if (next != current) {
    if (__builtin_setjmp(current->jmp_buf))
      return;

    // switch kstack
    tss.rsp0 = (unsigned long)next->kstack;

    // update current
    current = next;

    resume_task();
  }
}

void do_timer() {
  ticks++;

  // check timer
  FOREACH(node, timer_list) {
    struct task *t = STRUCT_FROM_FIELD(struct task, timer_node, node);
    if (t->alarm > ticks)
      continue;
    t->state = TASK_RUNNING;
    list_remove(node);
  }

  if (current != idle_task) {
    list_remove(&current->task_node);
    list_insert(&task_list, &current->task_node);
  }

  schedule();
}

__attribute__((interrupt))
void
timer_handler(struct interrupt_frame *frame) {
  outb(0x20, 0x20);
  do_timer();
}

int do_sleep(long ms) {
  current->alarm = ticks + ms / 10;
  list_insert(&timer_list, &current->timer_node);
  current->state = TASK_INTERRUPTIBLE;
  schedule();
  return 0;
}
