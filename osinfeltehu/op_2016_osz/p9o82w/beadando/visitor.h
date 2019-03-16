#ifndef VISITOR_H_INCLUDED
#define VISITOR_H_INCLUDED

#include <time.h>

typedef struct Visitor {
    time_t _time;
    char _name[255];
    char _email[255];
    char _id[255];
} Visitor;

void printVisitor(Visitor* visitor);
void set_visitorName(Visitor* vis, char* name);
void set_visitorEmail(Visitor* vis, char* email);
void set_visitorID(Visitor* vis, char* id);
//void set_visitorTime(Visitor* vis, time_t time);

#endif // VISITOR_H_INCLUDED
