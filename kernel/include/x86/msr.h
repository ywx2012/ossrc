#pragma once

#include <stdint.h>

#define MSR_EFER      0xc0000080
#define MSR_STAR      0xc0000081
#define MSR_LSTAR     0xc0000082
#define MSR_SFMASK    0xc0000084

#define MSR_EFER_SCE      0x0000000000000101
#define MSR_EFER_LME      0x0000000000000100

__attribute__((always_inline))
static
inline
void
wrmsr(uint32_t msr, uint64_t value) {
  uint32_t lo = value&0xFFFFFFFF;
  uint32_t hi = (value>>32)&0xFFFFFFFF;
  __asm__("wrmsr" : : "c"(msr), "a"(lo), "d"(hi));
}
