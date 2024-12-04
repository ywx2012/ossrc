#include <bsp.h>
#include <task.h>
#include <x86/pic.h>
#include <x86/pit.h>
#include <irq.h>

#define HZ 100

static size_t ticks = 0;

__attribute__((interrupt))
static
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

void
timer_init(void) {
  list_init(&idle.timer_node);
  irq_set_handler(IRQ_PIT0, 0, timer_handler);
  pit_set_mode(0, PIT_MODE_RATEGEN);
  pit_set_count(0, PIT_FREQ/HZ);
  pic_enable(IRQ_PIT0);
}

int do_sleep(long ms) {
  struct task *task = task_dequeue();
  task->wakeup = ticks + ms / (1000/HZ);
  list_insert(&idle.timer_node, &task->timer_node);
  task_yield();
  return 0;
}
