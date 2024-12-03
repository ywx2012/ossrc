#include <setup.h>
#include <bsp.h>

struct e820map e820map = {0};

struct dtr GDTR __attribute__((aligned(16))) = {
  .limit = sizeof(GDT) - 1,
  .base = (uintptr_t)GDT,
};

__attribute__((naked))
void
setup_start(void) {
  __asm__("mov %w0, %%ds\nmov %w0, %%es\nmov %w0, %%ss" : : "r"(0x10));
  __asm__("mov %w0, %%fs\nmov %w0, %%gs" : : "r"(0x0));
  __asm__("mov %0, %%rsp" : : "r"(bsp_stack + sizeof(bsp_stack)));
  main();
}
