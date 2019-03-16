#include "event.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

char* event_id(Event* event) {
    return event->_id;
}

void set_eventID(Event* event, char* id) {
    strcpy(event->_id, id);
}

void set_eventName(Event* event, char* name) {
    strcpy(event->_name, name);
}

void init_Event(Event* event) {
    event->_nextVisitor = 1;
    vector_init(&event->_visitors);
}
int add_visitorToEvent(Event* event, Visitor* vis) {
    vector_add(&event->_visitors, vis);
    return event->_nextVisitor++;
}

void delete_visitorFromVector(Event* event, void* i) {
    if(i != NULL) {
        printf("sdasasd");
        vector_delete(&event->_visitors, *(int *) i);
    }
}

int get_visitorSize(Event* event) {
    return vector_count(&event->_visitors);
}

Event* get_event(vector_* events, int i) {
    return (Event*) vector_get(events, i);
}

vector_* get_visitors(Event* event) {
    return &event->_visitors;
}

Visitor* get_visitor(Event* event, int i) {
    return (Visitor*) vector_get(&event->_visitors, i);
}

void printEvent(Event* event) {
    printf("%s ", event->_id);
    printf("%s\n", event->_name);
}

void event_free(vector_* events) {
    int i = 0;
    while(vector_count(events) < i) {
        vector_free(get_visitors((Event*) vector_get(events, i)));
    }
    vector_free(events);
}
