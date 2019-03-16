#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "visitor.h"

void printVisitor(Visitor* visitor) {
    printf("%ld ", visitor->_time);
    printf("%s ", visitor->_name);
    printf("%s\n", visitor->_email);
}

void set_visitorName(Visitor* vis, char* name) {
    strcpy(vis->_name, name);
}

void set_visitorEmail(Visitor* vis, char* email) {
    strcpy(vis->_email, email);
}

void set_visitorID(Visitor* vis, char* id) {
    strcpy(vis->_id, id);
}

/*void set_visitorTime(Visitor* vis, time_t time) {
    vis->_time = time;
}*/
