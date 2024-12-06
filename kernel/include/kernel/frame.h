#pragma once

#include <stdint.h>
#include <stddef.h>

void *frame_alloc(size_t n);
void frame_free(void *);
void frame_reclaim(uintptr_t start, uintptr_t end);
