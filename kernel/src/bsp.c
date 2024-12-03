#include <bsp.h>

char bsp_stack[BSP_STACK_SIZE] __attribute__((aligned(PAGE_SIZE)));
uintptr_t va_offset;

struct segment GDT[GDT_SIZE] = {
  [1]=CODESEG(0),
  [2]=DATASEG(0),
  [4]=DATASEG(3),
  [5]=CODESEG(3),
};

int main();

uintptr_t
va_from_pa(uintptr_t pa) {
  return pa+va_offset;
}

uintptr_t
pa_from_va(uintptr_t va) {
  return va-va_offset;
}

void
bsp_start(void) {
  frame_init();
  main();
}
