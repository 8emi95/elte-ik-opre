#include "visitor.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

visitor_t create_visitor(unsigned event_id, const char* name, const char* email) {
  visitor_t visitor;
  visitor.event_id = event_id;
  memcpy(&(visitor.name), name, MAX_VISITOR_NAME_LENGTH);
  memcpy(&(visitor.email), email, MAX_VISITOR_EMAIL_LENGTH);
  visitor.register_time = time(NULL);
  return visitor;
}

visitor_t* create_visitor_dyn(unsigned event_id, const char* name, const char* email) {
  visitor_t *visitor_ref = malloc(sizeof(visitor_t));
  visitor_t visitor = create_visitor(event_id, name, email);
  memcpy(visitor_ref, &visitor, sizeof(visitor_t));
  return visitor_ref;
}
