#pragma once

#include <stdint.h>

#define SYS_TIMER_SLEEP  0
#define SYS_SHM_MAP      1
#define SYS_SHM_GET_INFO 2

static
inline
int
syscall(uintptr_t number, uintptr_t arg1, uintptr_t arg2) {
  int result;
  __asm__("syscall"
          : "=a"(result)
          : "D"(arg1), "S"(arg2), "d"(number)
          : "rcx", "r8", "r9", "r10", "r11", "memory");
  return result;
}
