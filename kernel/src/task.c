#include <stdlib.h>
#include <string.h>
#include <x86/msr.h>
#include <x86/segment.h>
#include <kernel/task.h>
#include <kernel/bsp.h>
#include <kernel/paging.h>
#include <kernel/frame.h>

#define RIP 1
#define RSP 2
#define RFLAGS_IF 0x200
#define TASK_RSP0  0xFFFFFF8000000000

static struct segment gdt[10] = {
  [KERNEL_CS_INDEX]=CODESEG(0),
  [KERNEL_SS_INDEX]=DATASEG(0),
  [USER_SS_INDEX]=DATASEG(3),
  [USER_CS_INDEX]=CODESEG(3),
};

static struct dtr gdtr __attribute__((aligned(16))) = {
  .base = (uintptr_t)gdt,
  .limit = sizeof(gdt) - 1,
};

static struct tss tss = {
  .rsp = { [0] = TASK_RSP0 },
  .io_base = offsetof(struct tss, iomap),
  .iomap = {
    [sizeof(tss.iomap)-1] = 0xff,
  },
};

static uintptr_t next_id = 0;
struct task idle;
struct task *current;

#define USER_START 0x100000

__attribute__((naked,noreturn))
static
void
user_start(void) {
  __asm__("mov %c0, %%r11\n"
          "mov $0x8000000, %%rsp\n"
          "sysretq"
          : : "i"(RFLAGS_IF), "c"(USER_START));
}

__attribute__((naked))
static
void
syscall_handler(void) {
  __asm__("movq %%rsp, %%r10\n"
          "movq (tss+4)(%%rip), %%rsp\n"
          "pushq %%r10\n"
          "pushq %%rcx\n"
          "pushq %%r11\n"
          "movabs %0, %%r10\n"
          "call *%%r10\n"
          "popq %%r11\n"
          "popq %%rcx\n"
          "popq %%rsp\n"
          "sysretq"
          :
          : "i"(system_call)
          :"r10");
}

__attribute__((naked,noreturn))
static
void
idle_start(void) {
  for(;;)
    __asm__("sti; hlt");
}

__attribute__((naked,noipa))
static
void
load_gdt(void) {
  __asm__("lgdt %0" : : "m"(gdtr));
  __asm__("popq %%rax\n"
          "pushq $%c0\n"
          "pushq %%rax\n"
          "lretq\n"
          : : "i"(KERNEL_CS) : "rax", "memory");
}

void
task_init(void) {
  load_gdt();
  __asm__("mov %w0, %%ss; mov %w0, %%ds; mov %w0, %%es" : : "r"(KERNEL_SS));
  __asm__("mov %w0, %%fs; mov %w0, %%gs" : : "r"(0x0));

  uintptr_t base = (uintptr_t)&tss;
  struct segment tssd = TASKSEG(base, sizeof(tss));
  gdt[TSS_INDEX] = tssd;
  *(uint64_t *)(gdt+TSS_INDEX+1) = (base>>32);
  __asm__ ("ltr %w0" : : "r"(TSS));

  uint64_t star_val = ((uint64_t)USER_CS32) << 48  | ((uint64_t)KERNEL_CS) << 32;
  wrmsr(MSR_STAR, star_val);
  wrmsr(MSR_LSTAR, (uintptr_t)syscall_handler);
  wrmsr(MSR_SFMASK, RFLAGS_IF);

  uintptr_t *page_table = paging_alloc_table();
  paging_alloc_page(page_table, TASK_RSP0-PAGE_SIZE, PTE_W);

  idle.id = next_id++;
  idle.pml4 = page_table;
  idle.jmp_buf[RSP] = TASK_RSP0;
  idle.jmp_buf[RIP] = (uintptr_t)idle_start;
  list_init(&idle.task_node);
  current = &idle;
}

uintptr_t
task_create(size_t size, char const *data) {
  uintptr_t id = next_id++;
  uintptr_t *page_table = paging_alloc_table();
  paging_alloc_page(page_table, TASK_RSP0-PAGE_SIZE, PTE_W);

  for (size_t offset=0; offset<size; offset+=PAGE_SIZE) {
    void *page = paging_alloc_page(page_table, USER_START+offset, PTE_W|PTE_U);
    uintptr_t remain = size - offset;
    if (remain > PAGE_SIZE)
      remain = PAGE_SIZE;
    memcpy(page, data+offset, remain);
  }

  struct task* task = malloc(sizeof(struct task));
  if (!task)
    return (uintptr_t)-1;
  task->id = id;
  task->pml4 = page_table;
  task->jmp_buf[RSP] = TASK_RSP0;
  task->jmp_buf[RIP] = (uintptr_t)user_start;
  task_enqueue(task);
  return id;
}

struct task *
task_dequeue(void) {
  list_remove(&current->task_node);
  return current;
}

void
task_enqueue(struct task *task) {
  list_insert(&idle.task_node, &task->task_node);
}

void
task_yield() {
  if (!__builtin_setjmp(current->jmp_buf))
    task_switch();
}

static
__attribute__((noipa,naked))
void
resume(uintptr_t pa) {
  __asm__("mov %0, %%cr3" : : "a"(pa));
  __builtin_longjmp(current->jmp_buf, 1);
}

__attribute__((noipa,naked))
void
task_switch() {
  struct node *node = current->task_node.next;
  if (node == &idle.task_node)
    node = node->next;
  struct task *next = STRUCT_FROM_FIELD(struct task, task_node, node);
  current = next;
  uintptr_t pa = pa_from_ptr(current->pml4);
  resume(pa);
}
