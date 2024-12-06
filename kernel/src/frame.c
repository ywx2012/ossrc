#include <string.h>
#include <kernel/frame.h>
#include <kernel/bsp.h>

struct frame {
  size_t pages;
  struct node node;
};

static struct node frame_list;

static
uintptr_t
align_down(uintptr_t addr, uintptr_t size) {
  return addr-addr%size;
}

static
uintptr_t
align_up(uintptr_t addr, uintptr_t size) {
  return align_down(addr+size-1, size);
}

void
frame_init(void) {
  list_init(&frame_list);

  FOREACH(node, region_list) {
    struct region *region = STRUCT_FROM_FIELD(struct region, node, node);
    uintptr_t start = align_up(region->start, PAGE_SIZE);
    uintptr_t end = align_down(region->end, PAGE_SIZE);
    if (end <= start)
      continue;
    struct frame *frame = (struct frame *)ptr_from_pa(start);
    frame->pages = (end-start)/PAGE_SIZE;
    list_insert(&frame_list, &(frame->node));
  }
}

void *
frame_alloc(size_t n) {
  FOREACH(node, frame_list) {
    struct frame *frame = STRUCT_FROM_FIELD(struct frame, node, node);
    if (frame->pages < n)
      continue;
    frame->pages -= n;
    void *page = ((char *)frame) + PAGE_SIZE * (frame->pages);
    if (frame->pages == 0)
      list_remove(node);
    memset(page, 0, PAGE_SIZE * n);
    return page;
  }

  return NULL;
}

void
frame_free(void *page) {
  struct frame *frame = (struct frame *)page;
  frame->pages = 1;
  list_insert(&frame_list, &(frame->node));
}

void
frame_reclaim(uintptr_t start, uintptr_t end) {
  start = align_down(start, PAGE_SIZE);
  end = align_up(end, PAGE_SIZE);
  struct frame *frame = (struct frame *)ptr_from_pa(start);
  frame->pages = (end-start)/PAGE_SIZE;
  list_insert(&frame_list, &(frame->node));
}
