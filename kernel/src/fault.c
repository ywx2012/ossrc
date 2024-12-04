#include <x86/idt.h>
#include <kernel/task.h>
#include <kernel/paging.h>
#include <kernel/bsp.h>

__attribute__((interrupt))
static
void
pf_handler(struct interrupt_frame *frame __attribute__((unused)), uintptr_t error_code __attribute__((unused))) {
  uintptr_t addr;
  __asm__("mov %%cr2, %0" : "=r"(addr));
  paging_alloc_page(current->pml4, addr, PTE_W|PTE_U);
}

void
fault_init(void) {
  idt_set_exc_handler(14, 0, pf_handler);
}
