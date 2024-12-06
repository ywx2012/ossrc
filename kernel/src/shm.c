
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <user/shm.h>
#include <kernel/frame.h>
#include <kernel/paging.h>
#include <kernel/task.h>
#include <kernel/bsp.h>

struct shm {
  char* name;
  void *page;
  struct node shm_node;
};

static struct node shm_list;

void
shm_init(void) {
  list_init(&shm_list);
}

int
shm_map(char const* name, void *ptr) {
  uintptr_t va = (uintptr_t)ptr;
  struct shm* shm = NULL;

  FOREACH(node, shm_list) {
    struct shm *s = STRUCT_FROM_FIELD(struct shm, shm_node, node);
    if (!strcmp(s->name, name)) {
      shm = s;
      break;
    }
  }

  if (!shm) {
    shm = malloc(sizeof(struct shm));
    if (!shm)
      return -1;

    size_t len = strlen(name);
    shm->name = malloc(len + 1);
    if (!(shm->name)) {
      free(shm);
      return -1;
    }
    memcpy(shm->name, name, len);
    shm->name[len] = '\0';  
    shm->page = frame_alloc();
    list_insert(&shm_list, &shm->shm_node);
  }

  paging_map_page(current->pml4, va, shm->page, PTE_W|PTE_U);

  return 0;
}
