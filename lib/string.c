
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include "include/types.h"

void* memset(void *s, char c, unsigned long n) {
  char *tmp = s;

  while (n--) {
    *tmp++ = c;
  }

  return s;
}

void memcpy(void *dest, const void *src, unsigned long n) {
  char *tmp = dest;
  const char *s = src;

  while (n--) {
    *tmp++ = *s++;
  }
}

int strcmp(const char *s1, const char *s2) {
  unsigned char c1, c2;

  while (1) {
    c1 = *s1++;
    c2 = *s2++;
    if (c1 != c2) {
      return c1 < c2 ? -1 : 1;
    }
    if (!c1) {
      break;
    }
  }

  return 0;
}

int strlen(const char *s) {
  const char *sc;

  for (sc = s; *sc != '\0'; ++sc);

  return sc - s;
}

