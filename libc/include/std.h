
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <vesa.h>

void sleep(long ms);
void* shm_open(const char* name);
unsigned long fbmap();
void get_mode_info(struct mode_info* mode_info);
