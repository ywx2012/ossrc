#pragma once

#include <stddef.h>

void *memset(void *s, int c, size_t n);
void *memcpy(void * restrict dest, void const * restrict src, size_t n);
int strcmp(char const *s1, char const *s2);
size_t strlen(char const *s);
