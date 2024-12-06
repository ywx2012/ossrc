#pragma once

#include <stdint.h>

struct shm_info {
  uint32_t size;
  uint16_t xres;
  uint16_t yres;
};

int shm_map(char const *name, void const *ptr);
int shm_get_info(char const *name, struct shm_info *shm_info);
