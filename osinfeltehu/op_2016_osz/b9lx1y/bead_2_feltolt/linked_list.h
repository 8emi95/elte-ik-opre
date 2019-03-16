#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>

typedef struct node {
  void *data;
  struct node *next;
} node_t;

node_t *create_list_empty();
node_t *create_node(const void*, size_t);
void append_node(node_t**, const void*, size_t);
void push_node(node_t**, const void*, size_t);
int count_nodes(node_t**);
node_t *find_node(node_t**, void* (*)(void *), size_t, void*);
int delete_node(node_t**, const void*, void (*)(void*));
void delete_node_all(node_t**, const void*, void (*)(void*));
void free_nodes(node_t**, void (*)(void*));
void print_nodes(node_t**, void (*)(void*));

int write_nodes_to_file(node_t**, const char*, size_t);
int read_nodes_from_file(node_t**, const char*, size_t);
#endif
