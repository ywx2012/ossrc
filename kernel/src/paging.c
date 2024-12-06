#include <kernel/paging.h>
#include <kernel/frame.h>
#include <kernel/bsp.h>

uintptr_t *
paging_alloc_table(void) {
  uintptr_t *table = (uintptr_t *)frame_alloc(1);
  table[256] = pml4[256];
  return table;
}

void
paging_map_addr(uintptr_t *entry, uintptr_t va, uintptr_t pte) {
  char offsets[3] = {39, 30, 21};
  uintptr_t pa = PTE_ADDRESS(pte);
  uintptr_t attrs = pte & (PTE_W|PTE_U);

  for (size_t i=0; i<sizeof(offsets); ++i) {
    uint16_t index = (va >> offsets[i]) & 0x1FF;
    entry = entry + index;
    if (!((*entry) & PTE_P)) {
      void *page = frame_alloc(1);
      *entry = PTE_ADDRESS(pa_from_ptr(page))|PTE_P|attrs;
    }
    entry = (uintptr_t *)ptr_from_pa(PTE_ADDRESS(*entry));
  }

  uint16_t index = (va >> 12) & 0x1FF;
  entry[index] = pa|PTE_P|attrs;
}

void
paging_map_page(uintptr_t *entry, uintptr_t va, void *page, uintptr_t attrs) {
  paging_map_addr(entry, va, pa_from_ptr(page)|attrs);
}

void *
paging_alloc_page(uintptr_t *entry, uintptr_t va, uintptr_t attrs) {
  void *page = frame_alloc(1);
  paging_map_page(entry, va, page, attrs);
  return page;
}
