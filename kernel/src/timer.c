#include <bsp.h>
#include <task.h>
#include <x86/pic.h>
#include <interrupt.h>

static size_t ticks = 0;

void
timer_init(void) {
  list_init(&idle.timer_node);
  pic_enable(IRQ_PIT0);
}

__attribute__((interrupt))
void
timer_handler(struct interrupt_frame *frame) {
  if (!pic_acknowledge(IRQ_PIT0))
    return;

  ticks++;

  FOREACH(node, idle.timer_node) {
    struct task *task = STRUCT_FROM_FIELD(struct task, timer_node, node);
    if (task->wakeup > ticks)
      continue;
    list_remove(node);
    task_enqueue(task);
  }

  task_yield();
}

int do_sleep(long ms) {
  struct task *task = task_dequeue();
  task->wakeup = ticks + ms / 10;
  list_insert(&idle.timer_node, &task->timer_node);
  task_yield();
  return 0;
}
