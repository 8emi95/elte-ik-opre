#ifndef VISITOR_LIST_H
#define VISITOR_LIST_H

#include "visitor.h"
#include "linked_list.h"

#define VISITOR_SIZE (sizeof(visitor_t))

node_t* create_visitor_list_empty();
node_t* create_visitor_node(const visitor_t*);
void append_visitor(node_t**, const visitor_t*);
void push_visitor(node_t**, const visitor_t*);
node_t* find_visitor_by_event_id(node_t **, unsigned);
node_t* find_visitor_by_name(node_t **, const char *);
node_t* find_visitor_by_email(node_t **, const char *);
int delete_visitor(node_t**, unsigned);
void delete_visitor_all(node_t**, unsigned);
void free_visitors(node_t**);
void print_visitors(node_t**);

int write_visitors_to_file(node_t**, const char*);
int read_visitors_from_file(node_t**, const char*);

#endif
