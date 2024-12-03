#pragma once

#include <stdint.h>
#include <x86/page.h>

uintptr_t *paging_alloc_table(void);
void paging_map_addr(uintptr_t *entry, uintptr_t va, uintptr_t pa, uintptr_t attrs);
void paging_map_page(uintptr_t *entry, uintptr_t va, void *page, uintptr_t attrs);
void *paging_alloc_page(uintptr_t *entry, uintptr_t va, uintptr_t attrs);
