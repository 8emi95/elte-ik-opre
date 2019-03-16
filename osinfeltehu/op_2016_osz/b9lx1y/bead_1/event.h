#ifndef EVENT_H
#define EVENT_H

#define MAX_EVENT_NAME_LENGTH 64

typedef struct event {
  unsigned id;
  char name[MAX_EVENT_NAME_LENGTH];
} event_t;

event_t create_event(unsigned, const char*);
event_t *create_event_dyn(unsigned, const char*);

#endif
