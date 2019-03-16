#ifndef TYPES_H
#define TYPES_H

#include "vector.h"

typedef struct event {
    char name[40];
    char location[40];
    vector guests;
} Event;

typedef struct guest {
    char name[40];
    char mail[40];
    char entry_date[20];
} Guest;

Guest* guest_init();
Event* event_init();

static void get_timestamp();
void add_event(vector* events);
#endif
