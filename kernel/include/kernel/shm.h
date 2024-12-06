#pragma once

#include <list.h>
#include <user/shm.h>

struct shm {
  char name[8];
  uintptr_t pte;
  struct shm_info info;
  struct node shm_node;
};

int shm_create(struct shm *shm, char const* name);
