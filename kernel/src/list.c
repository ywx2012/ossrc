#include <list.h>

void
list_init(struct node *list) {
  list->prev = list;
  list->next = list;
}

void
list_insert(struct node *anchor, struct node *node) {
  struct node *prev = anchor->prev;
  node->prev = prev;
  node->next = anchor;
  prev->next = node;
  anchor->prev = node;
}

void
list_remove(struct node *node) {
  node->prev->next = node->next;
  node->next->prev = node->prev;
}
