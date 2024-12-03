#pragma once

#include <x86/page.h>

void *frame_alloc(void);
void frame_free(void *);
