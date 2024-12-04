
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <stddef.h>
#include <stdint.h>
#include <x86/page.h>
#include <kernel/frame.h>

struct bucket_desc {
  void* freeptr;
  struct bucket_desc* next; 
  short refcnt; 
};

struct bucket_dir {
  uint16_t size;
  struct bucket_desc* bdesc;
};

struct bucket_dir bucket_dir[] = {
  { 16, NULL }, 
  { 32, NULL },
  { 64, NULL },
  { 128, NULL },
  { 256, NULL },
  { 512, NULL },
  { 1024, NULL },
  { 2048, NULL },
  { 0, NULL }
};

void* malloc(int size) {
  void* va = NULL;
  struct bucket_dir* bdir = NULL;
  struct bucket_desc* bdesc = NULL;

  for (bdir = bucket_dir; bdir->size != 0; bdir++) {
    if (bdir->size >= size) {
      break;
    }
  }

  if (bdir->size == 0) {
    return NULL;
  }
  
  for (bdesc = bdir->bdesc; bdesc; bdesc = bdesc->next) {
    if (bdesc->freeptr) {
      break;
    }
  }
  
  if (bdesc == NULL) {
    bdesc = frame_alloc();
    bdesc->freeptr = (void*)((unsigned long)bdesc + sizeof(struct bucket_desc));
    bdesc->next = NULL;
    bdesc->refcnt = 0;

    unsigned long* p = bdesc->freeptr;
    int i = 0;
    while (++i < ((PAGE_SIZE - sizeof(struct bucket_desc)) / bdir->size)) {
      *p = (unsigned long)p + bdir->size;
      p = (unsigned long*)((unsigned long)p + bdir->size);
    }
    *p = 0;

    bdesc->next = bdir->bdesc;
    bdir->bdesc = bdesc;
  }
  
  va = bdesc->freeptr;
  bdesc->freeptr = (void*)(*((unsigned long*)va));
  bdesc->refcnt++;
  
  return va;
}

void free(void* obj) {
  unsigned long page = (unsigned long)obj & ~0xfffUL;
  struct bucket_dir* bdir = bucket_dir;;
  struct bucket_desc* bdesc = NULL;

  for (; bdir->size != 0; bdir++) {
    bdesc = bdir->bdesc;

    for (; bdesc; bdesc = bdesc->next) {
      if ((unsigned long)bdesc == page) {
        *((unsigned long*)obj) = (unsigned long)bdesc->freeptr;
        bdesc->freeptr = obj;
        bdesc->refcnt--;
        goto free_page;
      }
    }
  }

free_page:
  if (bdesc && bdesc->refcnt == 0) {
    struct bucket_desc* tmp = bdir->bdesc;
    struct bucket_desc* prev = NULL;
    for (; tmp; tmp = tmp->next) {
      if ((unsigned long)tmp == (unsigned long)bdesc) {
        break;
      }
      prev = tmp;
    }

    if (!prev) {
      bdir->bdesc = tmp->next;
    } else {
      prev->next = tmp->next;
    }

    frame_free(bdesc);
  }
}
