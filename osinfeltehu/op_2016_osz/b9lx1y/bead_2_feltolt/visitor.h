#ifndef VISITOR_H
#define VISITOR_H

#include <time.h>

#define MAX_VISITOR_NAME_LENGTH 64
#define MAX_VISITOR_EMAIL_LENGTH 64

typedef struct visitor {
  int event_id;
  char name[MAX_VISITOR_NAME_LENGTH];
  char email[MAX_VISITOR_EMAIL_LENGTH];
  time_t register_time;
} visitor_t;

visitor_t create_visitor(int, const char*, const char*);
visitor_t* create_visitor_dyn(int, const char*, const char*);

#endif
