
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <mm.h>
#include <print.h>
#include <sched.h>
#include <tss.h>
#include <segment.h>
#include <interrupt.h>
#include <syscall.h>
#include <shm.h>
#include <vesa.h>
#include <atkbd.h>

int main() {
  interrupt_init();
  syscall_init(); 
  shm_init();
  vesa_init();
  atkbd_init();

  sched_init();
  tss_init();
  init_8254();

  resume_task();
}
