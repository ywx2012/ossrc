#include <setup/setup.h>
#include <setup/header.h>
#include <kernel/bsp.h>
#include <kernel/selector.h>

struct e820map e820map = {0};

struct dtr gdtr __attribute__((aligned(16))) = {
  .limit = sizeof(gdt) - 1,
  .base = (uintptr_t)gdt,
};


struct node region_list;
static struct region regions[E820MAX+2];
static size_t nr_region = 0;
struct cpio_newc_header const *initrd;
size_t initrd_size;

static
void
insert_region(struct node *node, uintptr_t start, uintptr_t end) {
  regions[nr_region].start = start;
  regions[nr_region].end = end;
  list_insert(node, &regions[nr_region].node);
  nr_region += 1;
}

static
void
add_region(uintptr_t start, uintptr_t size) {
  uintptr_t end = start + size;

  FOREACH(node, region_list) {
    struct region *region = STRUCT_FROM_FIELD(struct region, node, node);
    if (region->start > start) {
      insert_region(node, start, end);
      return;
    }
  }

  insert_region(&region_list, start, end);
}

static
void
combine_consecutive_region(void) {
  FOREACH(node, region_list) {
    struct region *current = STRUCT_FROM_FIELD(struct region, node, node);
    while (node->next != &region_list) {
      struct region *next = STRUCT_FROM_FIELD(struct region, node, node->next);
      if (current->end != next->start)
        break;
      current->end = next->end;
      list_remove(node->next);
    }
  }
}

static
void
remove_region(uintptr_t start, uintptr_t end) {
  FOREACH(node, region_list) {
    struct region *region = STRUCT_FROM_FIELD(struct region, node, node);
    if ((region->start <= start) && (end <= region->end)) {
      if (region->start < start)
        insert_region(node, region->start, start);
      region->start = end;
      if (end == region->end)
        list_remove(node);
    }
  }
}

__attribute__((noipa))
static
void
setup_main(void) {
  va_offset = ((uintptr_t)__kernel_start)-CODE32_START;

  list_init(&region_list);
  for (uint32_t i=0; i<e820map.nr_map; ++i)
    if (e820map.map[i].type == E820_RAM)
      add_region(e820map.map[i].addr, e820map.map[i].size);

  combine_consecutive_region();
  remove_region(pa_from_ptr(__kernel_start), pa_from_ptr(__kernel_end));
  remove_region((uintptr_t)__setup_start, (uintptr_t)__setup_end);

  uintptr_t ramdisk_image = setup_header.ramdisk_image;
  initrd = (struct cpio_newc_header const *)ramdisk_image;
  initrd_size = setup_header.ramdisk_size;
  remove_region(ramdisk_image, ramdisk_image + initrd_size);

  bsp_start();
}

__attribute__((naked))
void
setup_start(void) {
  __asm__("mov %w0, %%ss" : : "r"(KERNEL_SS));
  __asm__("mov %w0, %%ds; mov %w0, %%es" : : "r"(USER_SS));
  __asm__("mov %w0, %%fs; mov %w0, %%gs" : : "r"(0x0));
  __asm__("mov %0, %%rsp" : : "r"(bsp_stack + sizeof(bsp_stack)));
  setup_main();
}
