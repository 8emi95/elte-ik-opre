#include "visitor_list.h"

#include <stdio.h>
#include <string.h>

void free_visitor(void*);
void print_visitor(void*);
void* visitor_event_id_accessor(void*);
void* visitor_name_accessor(void*);
void* visitor_email_accessor(void*);

void free_visitor(void* data) {}

void print_visitor(void *data) {
  visitor_t *visitor = (visitor_t *)data;
  printf("--- Visitor ---\nID: %u\nNAME: %s\nEMAIL: %s\nREGISTER: %s\n",
         visitor->event_id,
         visitor->name,
         visitor->email,
         ctime(&visitor->register_time));
}

void* visitor_event_id_accessor(void *data) {
  return &(((visitor_t *)data)->event_id);
}

void* visitor_name_accessor(void *data) {
  return &(((visitor_t *)data)->name);
}

void* visitor_email_accessor(void *data) {
  return &(((visitor_t *)data)->email);
}

node_t* create_visitor_list_empty() {
  return create_list_empty();
}

node_t* create_visitor_node(const visitor_t *visitor) {
  return create_node(visitor, VISITOR_SIZE);
}

void append_visitor(node_t **head, const visitor_t *visitor) {
  append_node(head, visitor, VISITOR_SIZE);
}

void push_visitor(node_t **head, const visitor_t *visitor) {
  push_node(head, visitor, VISITOR_SIZE);
}

node_t* find_visitor_by_event_id(node_t **head, int event_id) {
  return find_node(head, &visitor_event_id_accessor, sizeof(event_id), &event_id);
}

int count_visitor_by_event_id(node_t **head, int event_id) {
  int count = 0;
  node_t *it = find_visitor_by_event_id(head, event_id);
  //printf("first node found: %p\n", it);
  while(it) {
    count++;
    it = find_visitor_by_event_id(&(it->next), event_id);
  }
  return count;
}

node_t* find_visitor_by_name(node_t **head, const char *name) {
  return find_node(head, &visitor_name_accessor, strlen(name) + 1, (void*)name);
}

node_t* find_visitor_by_email(node_t **head, const char *email) {
  return find_node(head, &visitor_email_accessor, strlen(email) + 1, (void*)email);
}

int delete_visitor(node_t **head, int event_id) {
  node_t *current = *head, *prev; 

  if (current &&
      ((visitor_t *)(current->data))->event_id == event_id) {
    *head = current->next;
    free_visitor(current->data);
    free(current->data);
    free(current);
    return 1;
  }

  while (current &&
        ((visitor_t *)(current->data))->event_id != event_id) {
    prev = current;
    current = current->next; 
  }

  if (!current)
    return 0;

  prev->next = current->next;
  free_visitor(current->data);
  free(current->data);
  free(current);
  return 1;
}
  
void delete_visitor_all(node_t **head, int event_id) {
  while(delete_visitor(head, event_id));
}

void free_visitors(node_t **head) {
  free_nodes(head, &free_visitor);
}

void print_visitors(node_t **head) {
  print_nodes(head, &print_visitor);
}

int write_visitors_to_file(node_t **head, const char* file_name) {
  return write_nodes_to_file(head, file_name, VISITOR_SIZE);
}

int read_visitors_from_file(node_t **head, const char* file_name) {
  return read_nodes_from_file(head, file_name, VISITOR_SIZE);
}

