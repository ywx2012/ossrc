
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <string.h>
#include <vesa.h>
#include <bochs.h>
#include <mm.h>
#include <sched.h>

struct mode_info vesa_mode_info;

void vesa_init() {
  bochs_init(&vesa_mode_info);
} 

unsigned long do_get_mode_info(struct mode_info *mode_info) {
  mode_info->fbbase = vesa_mode_info.fbbase;
  mode_info->hres = vesa_mode_info.hres;
  mode_info->vres = vesa_mode_info.vres;
  mode_info->bpp = vesa_mode_info.bpp;

  return 0;
}

unsigned long do_fbmap() {
  unsigned long va = 0xe000000;
  unsigned long pa = vesa_mode_info.fbbase;
  unsigned long size = vesa_mode_info.hres * vesa_mode_info.vres * vesa_mode_info.bpp / 8;
  for(unsigned long offset = 0; offset<size; offset+=PAGE_SIZE)
    map_page(current->pml4, va+offset, pa+offset, 0x4);

  return va;
}