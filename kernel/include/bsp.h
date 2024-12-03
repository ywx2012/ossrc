#pragma once

#include <stdint.h>
#include <stddef.h>
#include <x86/page.h>
#include <x86/segment.h>
#include <cpio.h>
#include <list.h>

#define BSP_STACK_SIZE PAGE_SIZE
extern char bsp_stack[BSP_STACK_SIZE];

#define GDT_SIZE 10
extern struct segment GDT[GDT_SIZE];

struct region {
  uintptr_t start;
  uintptr_t end;
  struct node node;
};


extern char const __kernel_start[];
extern char const __kernel_end[];

extern uintptr_t va_offset;
extern struct cpio_newc_header const *initrd;
extern size_t initrd_size;

extern struct node region_list;

extern uintptr_t pml4[PAGE_SIZE/sizeof(uintptr_t)];

void frame_init(void);

void bsp_start(void);
uintptr_t va_from_pa(uintptr_t pa);
uintptr_t pa_from_va(uintptr_t va);
