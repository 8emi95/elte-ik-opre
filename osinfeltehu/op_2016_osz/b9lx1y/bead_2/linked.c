#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_VISITOR_NAME_LENGTH 255
#define MAX_EVENT_NAME_LENGTH 255

enum user_action {
  CREATE_VISITOR,
  READ_VISITOR,
  UPDATE_VISITOR,
  DELETE_VISITOR,
  
  CREATE_EVENT,
  READ_EVENT,
  UPDATE_EVENT,
  DELETE_EVENT  
};

typedef struct event {
  int id;
  char name[MAX_VISITOR_NAME_LENGTH];
} event_t;

event_t create_event(int , const char*);

typedef struct event_node {
  event_t event;
  struct event_node *next;
} event_node_t;

event_node_t* create_event_list_empty();
event_node_t* create_event_node(const event_t*);
void append_event(event_node_t**, const event_t*);
void push_event(event_node_t**, const event_t*);
void delete_event(event_node_t**, int );
void free_events(event_node_t**);
void print_events(const event_node_t*);

typedef struct visitor {
  int event_id;
  char name[MAX_EVENT_NAME_LENGTH];
} visitor_t;

typedef struct visitor_node {
  visitor_t visitor;
  struct visitor_node *next;
} visitor_node_t;

event_node_t* create_event_list_empty() {
  return NULL;
}

event_node_t* create_event_node(const event_t *event) {
  event_node_t *new_node = malloc(sizeof(event_node_t));
  memcpy(&(new_node->event), event, sizeof(new_node->event));
  new_node->next = NULL;
  return new_node;
}

void append_event(event_node_t **head, const event_t *event) {
  event_node_t *new_node = create_event_node(event);

  if (!*head) {
    *head = new_node;
    return;
  }

  event_node_t *current = *head, *prev = NULL; 

  while (current) {
    prev = current;
    current = current->next;
  }

  prev->next = new_node;
}

void push_event(event_node_t **head, const event_t *event) {
  event_node_t *new_node = create_event_node(event);

  if (!*head) {
    *head = new_node;
    return;
  }

  new_node->next = *head;
  *head = new_node;
}

void delete_event(event_node_t **head, int id) {
  event_node_t *current = *head, *prev = NULL; 

 if (current && current->event.id == id) {
   *head = current->next;
   free(current);
   return;
 }

 while (*head && (*head)->event.id != id) {
  prev = current;
  current = current->next; 
 }

 if (!current)
   return;

 prev->next = current->next;
 free(current);
}

void free_events(event_node_t **head) {
  event_node_t *current = *head, *prev;
  while (current) {
    prev = current;
    current = current->next;
    free(prev);
  }
  *head = NULL;
}

void print_events(const event_node_t *head) {
  const event_node_t *current = head;
  int n = 0;
  while (current) {
    printf("Event %u\n", n);
    printf("ID: %u\n", current->event.id);
    printf("Name: %s\n", current->event.name);
    current = current->next;
    n++;
  }
}

event_t create_event(int id, const char* name) {
  event_t new_event;
  new_event.id = id;
  memcpy(&new_event.name, name, sizeof(new_event.name));
  return new_event;
}

int main() {
  event_node_t * head = create_event_list_empty();

  event_t event1 = create_event(1, "Imperial wedding");
  event_t event2 = create_event(2, "Sparring in Rome");
  event_t event3 = create_event(3, "Fine dining in Paris");

  append_event(&head, &event1);
  append_event(&head, &event2);
  append_event(&head, &event3);

  print_events(head);

  free_events(&head);  
  return 0;
}
