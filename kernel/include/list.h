#pragma once

#include <stdint.h>

struct node {
  struct node *prev;
  struct node *next;
};

#define STRUCT_FROM_FIELD(type,field,ptr) ((type *)(((uintptr_t)ptr)-offsetof(type, field)))
#define FOREACH(var,list) for (struct node *var=list.next; var!=&list; var=var->next)

void list_init(struct node *list);
void list_insert(struct node *anchor, struct node *node);
void list_remove(struct node *node);
