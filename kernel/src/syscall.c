
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <syscall.h>
#include <x86/msr.h>
#include <stdint.h>
#include <segment.h>
#include <sched.h>
#include <vesa.h>
#include <tss.h>

#define RF_IF 0x00000200

// this declare fn_ptr can have any number any type args
typedef unsigned long (*fn_ptr)();

unsigned long do_sleep(long ms);
unsigned long do_shm(const char* name);
unsigned long do_fbmap();
unsigned long do_get_mode_info(struct mode_info *mode_info);

fn_ptr syscall_table[] = { do_sleep, do_shm, do_fbmap, do_get_mode_info };

__attribute__((naked))
void
system_call() {
  uintptr_t number;
  __asm__("" : "=a"(number));
  __asm__("movq %%rsp, (tss+20)(%%rip)\n"
          "movq (tss+4)(%%rip), %%rsp\n"
          "pushq (tss+20)(%%rip)\n"
          "pushq %%rcx\n"
          "pushq %%r11\n"
          "call *%0\n"
          "popq %%r11\n"
          "popq %%rcx\n"
          "popq %%rsp\n"
          "sysretq"
          : : "r"(syscall_table[number]));
}

void syscall_init() {
  uint64_t star_val = ((uint64_t)USER32_CS) << 48  | ((uint64_t)KERNEL_CS) << 32;
  wrmsr(MSR_STAR, star_val);
  wrmsr(MSR_LSTAR, (uint64_t)system_call);
  wrmsr(MSR_SFMASK, RF_IF);
}
