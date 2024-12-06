#include <string.h>
#include <kernel/bsp.h>
#include <kernel/shm.h>
#include <kernel/frame.h>

static struct shm font1;

static
size_t
align_down(size_t n, size_t size) {
  return n - (n % size);
}

static
size_t
align_up(size_t n, size_t size) {
  return align_down(n + (size - 1), size);
}

void
font_init(void) {
  font1.info = font_info;
  size_t size = align_up(font_info.size, PAGE_SIZE)/PAGE_SIZE;
  void *page = frame_alloc(size);
  memcpy(page, glyphs, font_info.size);
  font1.pte = pa_from_ptr(page);
  shm_create(&font1, "font-1");
}
