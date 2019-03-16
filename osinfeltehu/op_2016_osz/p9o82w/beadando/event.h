#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

#include "visitor.h"
#include "vector.h"

#define EVENT_ID_SIZE 32
#define EVENT_NAME_SIZE 255

typedef struct Event {
    vector_ _visitors;
    int _nextVisitor;
    char _id[EVENT_ID_SIZE];
    char _name[EVENT_NAME_SIZE];
} Event;

void init_Event(Event* event);
void set_eventID(Event* event, char* id);
void set_eventName(Event* event, char* name);
void delete_visitorFromVector(Event* event, void* i);
void printEvent(Event* event);
void create_Event(Event* event, char* name, char* id);
void event_free(vector_* events);
char* event_id(Event* event);
int get_visitorSize(Event* event);
int add_visitorToEvent(Event* event, Visitor* vis);
Visitor* get_visitor(Event* event, int i);
Event* get_event(vector_* events, int i);
vector_* get_visitors(Event* event);


#endif // EVENT_H_INCLUDED
