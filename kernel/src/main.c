
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <print.h>
#include <sched.h>
#include <interrupt.h>
#include <syscall.h>
#include <shm.h>
#include <vesa.h>
#include <atkbd.h>

int main() {
  syscall_init(); 
  shm_init();
  vesa_init();
  atkbd_init();

  init_8254();
}
