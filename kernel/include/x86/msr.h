#pragma once

#include <stdint.h>

#define MSR_IA32_SYSENTER_CS   0x174
#define MSR_IA32_SYSENTER_ESP  0x175
#define MSR_IA32_SYSENTER_EIP  0x176

static
inline
void
wrmsr(uint32_t msr, uint64_t value) {
  uint32_t lo = value&0xFFFFFFFF;
  value >>= 32;
  uint32_t hi = value&0xFFFFFFFF;
  __asm__("wrmsr" : : "c"(msr), "a"(lo), "d"(hi));
}
