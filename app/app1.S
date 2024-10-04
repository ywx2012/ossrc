
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

.text
.code64
1:
  mov $0x3f8, %dx
  mov $'A', %ax
  out %ax, %dx
  jmp 1b
