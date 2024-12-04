
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <task.h>
#include <list.h>
#include <frame.h>
#include <paging.h>

struct shm {
  char* name;
  void *page;
  struct node shm_node;
};

struct node shm_list;

void
shm_init(void) {
  list_init(&shm_list);
}

int do_shm(char* name) {
  struct shm* shm = NULL;
  unsigned long va = 0x4000000;

  FOREACH(node, shm_list) {
    struct shm * s=STRUCT_FROM_FIELD(struct shm, shm_node, node);
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
    shm->page = frame_alloc();
    list_insert(&shm_list, &shm->shm_node);
  }

  paging_map_page(current->pml4, va, shm->page, PTE_W|PTE_U);

  return va;
}
