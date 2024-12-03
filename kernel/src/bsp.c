#include <bsp.h>

char bsp_stack[BSP_STACK_SIZE] __attribute__((aligned(PAGE_SIZE)));

struct segment GDT[GDT_SIZE] = {
  [1]=CODESEG(0),
  [2]=DATASEG(0),
  [4]=DATASEG(3),
  [5]=CODESEG(3),
};
