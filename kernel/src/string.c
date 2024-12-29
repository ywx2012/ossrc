
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <kernel/string.h>

void *
memset(void *s, int c, size_t n) {
  char *t = s;
  for (size_t i=0; i<n; ++i)
    t[i] = (char)c;
  return s;
}

void *
memcpy(void * restrict dest, void const * restrict src, size_t n) {
  char *d = dest;
  char const *s = src;
  for (size_t i=0; i<n; ++i)
    d[i] = s[i];
  return dest;
}

int
strcmp(char const *s1, char const *s2) {
  int c;
  unsigned char c1, c2;
  do {
    c1 = (unsigned char)*s1++;
    c2 = (unsigned char)*s2++;
    c = (c1>c2)-(c1<c2);
  } while ((!c) && c1);
  return c;
}

size_t
strlen(char const *s) {
  size_t i=0;
  while(s[i])
    ++i;
  return i;
}

