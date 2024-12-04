
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <print.h>
#include <syscall.h>
#include <shm.h>
#include <atkbd.h>

int main() {
  syscall_init(); 
  shm_init();
  atkbd_init();
}
