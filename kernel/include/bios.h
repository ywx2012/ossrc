#pragma once

#include <stdint.h>
#include <asm/e820.h>

void bios_load_map(struct e820map *map);
void bios_load_font(uint8_t *font);
