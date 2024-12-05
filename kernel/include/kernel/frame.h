#pragma once

#include <stdint.h>

void *frame_alloc(void);
void frame_free(void *);
void frame_reclaim(uintptr_t start, uintptr_t end);
