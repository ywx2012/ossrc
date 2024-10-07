
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include "include/syscall.h"
#include <stdint.h>
#include "include/segment.h"
#include "include/sched.h"
#include "include/vesa.h"

#define MSR_STAR 0xc0000081
#define MSR_LSTAR 0xc0000082
#define MSR_SYSCALL_MASK 0xc0000084
#define RF_IF 0x00000200

// this declare fn_ptr can have any number any type args
typedef unsigned long (*fn_ptr)();

unsigned long do_sleep(long ms);
unsigned long do_shm(const char* name);
unsigned long do_fbmap();
unsigned long do_get_mode_info(struct mode_info *mode_info);

fn_ptr syscall_table[] = { do_sleep, do_shm, do_fbmap, do_get_mode_info };

void syscall_init() {
  uint64_t star_val = (uint64_t)USER32_CS << 48  | (uint64_t)KERNEL_CS << 32;
  uint64_t syscall_entry = (uint64_t)system_call;
  uint64_t syscall_mask = RF_IF;

  __asm__("wrmsr": : "c" (MSR_STAR), "a" ((uint32_t)star_val), "d" (star_val >> 32));
  __asm__("wrmsr": : "c" (MSR_LSTAR), "a" ((uint32_t)syscall_entry), "d" (syscall_entry >> 32));
  __asm__("wrmsr": : "c" (MSR_SYSCALL_MASK), "a" ((uint32_t)syscall_mask), "d" (syscall_mask >> 32));
}

