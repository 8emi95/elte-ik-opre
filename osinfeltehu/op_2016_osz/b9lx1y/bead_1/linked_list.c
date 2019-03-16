#include "linked_list.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

node_t* create_list_empty() {
  return NULL;
}

node_t* create_node(const void *data, size_t size) {
  node_t *new_node = malloc(sizeof(node_t));
  new_node->data = malloc(size);
  memcpy(new_node->data, data, size);
  new_node->next = NULL;
  return new_node;
}

void append_node(node_t **head, const void *data, size_t size) {
  node_t *new_node = create_node(data, size);

  if (!*head) {
    *head = new_node;
    return;
  }

  node_t *current = *head, *prev; 

  while (current) {
    prev = current;
    current = current->next;
  }

  prev->next = new_node;
}

void push_node(node_t **head, const void *data, size_t size) {
  node_t *new_node = create_node(data, size);

  if (!*head) {
    *head = new_node;
    return;
  }

  new_node->next = *head;
  *head = new_node;
}

unsigned count_nodes(node_t **head) {
  node_t *current = *head;
  unsigned count = 0;
  while (current) {
    ++count;
    current = current->next;
  }
  return count;
}

node_t* find_node(node_t **head, void* (*accessor)(void *), size_t size, void* compare_address) {
  node_t *current = *head;
  
  if (current &&
    0 == memcmp((*accessor)(current->data), compare_address, size)) {
    return current;
  }

  while (current &&
    0 != memcmp((*accessor)(current->data), compare_address, size)) {
    current = current->next; 
  }
  
  return current;
}

int delete_node(node_t **head, const void *data, void (*free_data)(void*)) {
  node_t *current = *head, *prev; 

  if (current && current->data == data) {
    *head = current->next;
    (*free_data)(current->data);
    free(current->data);
    free(current);
    return 1;
  }

  while (current && current->data != data) {
    prev = current;
    current = current->next; 
  }

  if (!current)
    return 0;

  prev->next = current->next;
  (*free_data)(current->data);
  free(current->data);
  free(current);
  return 1;
}

void delete_node_all(node_t **head, const void *data, void (*free_data)(void*)) {
  while (delete_node(head, data, free_data));
}

void free_nodes(node_t **head, void (*free_data)(void*)) {
  node_t *current = *head, *prev;
  while (current) {
    prev = current;
    current = current->next;
    (*free_data)(prev->data);
    free(prev->data);
    free(prev);
  }
  *head = NULL;
}

void print_nodes(node_t **head, void (*print_node)(void*)) {
  const node_t *current = *head;

  if (!current)
    printf("A lista ures!\n");

  while (current) {
    (*print_node)(current->data);
    current = current->next;
  }
}


int write_nodes_to_file(node_t **head, const char* file_name, size_t size) {
  int fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
  if (fd < 0) {
    perror("open");
    return fd;
  }

  node_t *current = *head;
  while (current) {
    if (write(fd, current->data, size) != (ssize_t)size) {
      perror("write"); 
      return -1;
    }
    current = current->next;
  }
  
  close(fd);

  return 0;
}

int read_nodes_from_file(node_t **head, const char* file_name, size_t size) {
  int fd = open(file_name, O_CREAT | O_RDONLY, 0644);
  if (fd < 0) {
    perror("open");
    return fd;
  }

  ssize_t read_bytes;
  char data[size]; 
  while ((read_bytes = read(fd, &data, size))) {
    if (read_bytes < (ssize_t)size) {
      const char error_msg[] = "Invalid file error\n";
      write(2, &error_msg, sizeof(error_msg));
      return -1;
    }
    append_node(head, &data, size);
  }
  
  close(fd);

  return 0;
}
