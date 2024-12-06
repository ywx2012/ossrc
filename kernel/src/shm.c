
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <string.h>
#include <kernel/shm.h>
#include <kernel/frame.h>
#include <kernel/paging.h>
#include <kernel/task.h>
#include <kernel/bsp.h>

static struct node shm_list;
static struct shm shm1;

int
shm_create(struct shm *shm, char const* name) {
  size_t len = strlen(name);
  if (len >= 8)
    return -1;
  memset(shm->name, 0, sizeof(shm->name));
  memcpy(shm->name, name, len);
  list_insert(&shm_list, &shm->shm_node);
  return 0;
}

void
shm_init(void) {
  list_init(&shm_list);
  shm1.info.size = PAGE_SIZE;
  shm1.info.xres = 1;
  shm1.info.yres = 1;
  shm1.pte = pa_from_ptr(frame_alloc(1))|PTE_W;
  shm_create(&shm1, "shm-1");
}

static
struct shm *
find_shm(char const* name) {
  FOREACH(node, shm_list) {
    struct shm *shm = STRUCT_FROM_FIELD(struct shm, shm_node, node);
    if (!strcmp(shm->name, name))
      return shm;
  }
  return NULL;
}

int
shm_map(char const* name, void const *ptr) {
  uintptr_t va = (uintptr_t)ptr;
  struct shm *shm = find_shm(name);
  if (!shm)
    return -1;
  for (size_t offset=0; offset<shm->info.size; offset+=PAGE_SIZE)
    paging_map_addr(current->pml4, va+offset, (shm->pte+offset)|PTE_U);
  return 0;
}

int
shm_get_info(char const* name, struct shm_info *info) {
  struct shm *shm = find_shm(name);
  if (!shm)
    return -1;
  *info = shm->info;
  return 0;
}
