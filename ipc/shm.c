
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "include/mm.h"
#include "include/sched.h"

struct shm {
  char* name;
  unsigned long page;
	
  struct shm* next;
};

struct shm* shm_head;
struct shm* shm_tail;

int do_shm(char* name) {
  struct shm* shm = NULL;
  unsigned long va = 0x4000000;

  for (struct shm* s = shm_head; s; s = s->next) {
    if (!strcmp(s->name, name)) {
      shm = s;
      break;
    }
  }

  if (!shm) {
    shm = malloc(sizeof(struct shm));
    int len = strlen(name);
    shm->name = malloc(len + 1);
    memcpy(shm->name, name, len);
    shm->name[len] = '\0';  
    shm->page = alloc_page();
    shm->next = NULL;

    if (shm_head == NULL) {
      shm_head = shm;
      shm_tail = shm;
    } else {
      shm_tail->next = shm;
      shm_tail = shm;
    }
  }

  map_page(current->pml4, va, shm->page, 0x4);

  return va;
}
