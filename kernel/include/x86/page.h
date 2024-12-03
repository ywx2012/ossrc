#pragma once

#define PAGE_SIZE 4096

#define PTE_P  0x01
#define PTE_W  0x02
#define PTE_U  0x04
#define PTE_PS 0x80

#define PTE_ADDRESS(x) ((x)&0xffffffffff000)
