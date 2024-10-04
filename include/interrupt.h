
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#pragma once

void init_8254();
void interrupt_init();

void timer_handler();
void pf_handler();
void kb_handler();

