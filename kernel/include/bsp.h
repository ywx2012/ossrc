#pragma once

#include <x86/page.h>
#include <x86/segment.h>


#define BSP_STACK_SIZE PAGE_SIZE
extern char bsp_stack[BSP_STACK_SIZE];

#define GDT_SIZE 10
extern struct segment GDT[GDT_SIZE];

int main();
