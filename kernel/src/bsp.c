#include <kernel/bsp.h>
#include <kernel/task.h>

char bsp_stack[BSP_STACK_SIZE] __attribute__((aligned(PAGE_SIZE)));
uintptr_t va_offset;

uintptr_t
va_from_pa(uintptr_t pa) {
  return pa+va_offset;
}

uintptr_t
pa_from_va(uintptr_t va) {
  return va-va_offset;
}

static
void
spawn_task(char const *filename) {
  struct cpio_newc_header const *cpio = cpio_lookup(initrd, filename);
  task_create(cpio_get_size(cpio), cpio_get_content(cpio));
}

void
bsp_start(void) {
  frame_init();
  task_init();
  fault_init();
  irq_init();
  timer_init();
  shm_init();
  atkbd_init();
  fb_init();
  idt_init();

  spawn_task("app1.bin");
  spawn_task("app2.bin");
  task_switch();
}
