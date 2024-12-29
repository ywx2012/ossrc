#pragma once

#include <stdint.h>

#define SYS_TIMER_SLEEP  0
#define SYS_SHM_MAP      1
#define SYS_SHM_GET_INFO 2

static inline
int
syscall(uintptr_t number, uintptr_t arg1, uintptr_t arg2) {
  int result = 0;
  __asm__ goto("mov %%esp, %%esi;"
               "mov $%l4, %%edi;"
               "sysenter"
               : "=a"(result)
               : "d"(arg1), "c"(arg2), "b"(number)
               : "edi", "esi", "memory"
               : next);
 next:
  return result;
}
