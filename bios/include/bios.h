#pragma once
#include <stdint.h>

#define E820MAX 128

struct e820entry {
  uint64_t addr;
  uint64_t size;
  uint32_t type;
} __attribute__((packed));

struct e820map {
  uint32_t nr_map;
  struct e820entry map[E820MAX];
};

void bios_load_map(struct e820map *map);
void bios_load_font(uint8_t *font);
