#pragma once

#include <stdint.h>
#include <stddef.h>
#include <x86/page.h>
#include <x86/segment.h>
#include <cpio.h>
#include <list.h>

#define BSP_STACK_SIZE PAGE_SIZE
extern char bsp_stack[BSP_STACK_SIZE];

struct region {
  uintptr_t start;
  uintptr_t end;
  struct node node;
};

extern char const __kernel_start[];
extern char const __kernel_end[];
extern char const __setup_start[];
extern char const __setup_end[];

extern uintptr_t va_offset;
extern struct cpio_newc_header const *initrd;
extern size_t initrd_size;

extern struct node region_list;

extern uintptr_t pml4[PAGE_SIZE/sizeof(uintptr_t)];

void frame_init(void);
void task_init(void);
void fault_init(void);
void irq_init(void);
void timer_init(void);
void shm_init(void);
void atkbd_init(void);
void fb_init(void);
void idt_init(void);

void bsp_start(void);
void *ptr_from_pa(uintptr_t pa);
uintptr_t pa_from_ptr(void const *);
