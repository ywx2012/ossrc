#include <list.h>

void
list_init(struct node *list) {
  list->prev = list;
  list->next = list;
}

void
list_insert(struct node *list, struct node *node) {
  struct node *last = list->prev;
  node->prev = last;
  node->next = list;
  last->next = node;
  list->prev = node;
}

void
list_remove(struct node *node) {
  node->prev->next = node->next;
  node->next->prev = node->prev;
}
