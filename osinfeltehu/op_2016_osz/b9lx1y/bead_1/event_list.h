#ifndef EVENT_LIST_H
#define EVENT_LIST_H

#include "event.h"
#include "linked_list.h"

#define EVENT_SIZE (sizeof(event_t))

node_t* create_event_list_empty();
node_t* create_event_node(const event_t*);
void append_event(node_t**, const event_t*);
void push_event(node_t**, const event_t*);
node_t* find_event_by_id(node_t **, unsigned);
node_t* find_event_by_name(node_t **, const char *);
int delete_event(node_t**, unsigned);
void delete_event_all(node_t**, unsigned);
void free_events(node_t**);
void print_events(node_t**);

int write_events_to_file(node_t**, const char*);
int read_events_from_file(node_t**, const char*);

#endif
