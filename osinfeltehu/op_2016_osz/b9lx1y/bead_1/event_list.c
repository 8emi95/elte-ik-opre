#include "linked_list.h"
#include "event.h"
#include "event_list.h"

#include <stdio.h>
#include <string.h>

void free_event(void*);
void print_event(void*);
void* event_id_accessor(void*);
void* event_name_accessor(void*);

void free_event(void *event) {}

void print_event(void *data) {
  event_t *event = (event_t *)data;
  printf("--- Event ---\nID: %u\nNAME: %s\n",
         event->id,
         event->name);
}

void* event_id_accessor(void *data) {
  return &(((event_t *)data)->id);
}

void* event_name_accessor(void *data) {
  return &(((event_t *)data)->name);
}

node_t* create_event_list_empty() {
  return create_list_empty();
}

node_t* create_event_node(const event_t *event) {
  return create_node(event, EVENT_SIZE);
}

void append_event(node_t **head, const event_t *event) {
  append_node(head, event, EVENT_SIZE);
}

void push_event(node_t **head, const event_t *event) {
  push_node(head, event, EVENT_SIZE);
}

node_t* find_event_by_id(node_t **head, unsigned id) {
  return find_node(head, &event_id_accessor, sizeof(id), &id);
}

node_t* find_event_by_name(node_t **head, const char *name) {
  return find_node(head, &event_name_accessor, strlen(name) + 1, (void*)name);
}

int delete_event(node_t **head, unsigned id) {
  node_t *current = *head, *prev; 

  if (current &&
      ((event_t *)(current->data))->id == id) {
    *head = current->next;
    free_event(current->data);
    free(current->data);
    free(current);
    return 1;
  }

  while (current &&
        ((event_t *)(current->data))->id != id) {
    prev = current;
    current = current->next; 
  }

  if (!current)
    return 0;

  prev->next = current->next;
  free_event(current->data);
  free(current->data);
  free(current);
  return 1;
}

void delete_event_all(node_t **head, unsigned id) {
  while(delete_event(head, id));
}
  
void free_events(node_t **head) {
  free_nodes(head, &free_event);
}

void print_events(node_t **head) {
  print_nodes(head, &print_event);
}

int write_events_to_file(node_t **head, const char* file_name) {
  return write_nodes_to_file(head, file_name, EVENT_SIZE);
}

int read_events_from_file(node_t **head, const char* file_name) {
  return read_nodes_from_file(head, file_name, EVENT_SIZE);
}

