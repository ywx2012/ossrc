
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Rserved.

#include <stdint.h>
#include <mm.h>
#include <frame.h>
#include <bsp.h>

unsigned long alloc_page() {
  return pa_from_va((uintptr_t)frame_alloc());
}
