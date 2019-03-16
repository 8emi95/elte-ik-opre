#include "event.h"

#include <stdlib.h>
#include <string.h>

event_t create_event(int id, const char* name) {
  event_t event;
  event.id = id;
  memcpy(&(event.name), name, MAX_EVENT_NAME_LENGTH);
  return event;
}

event_t* create_event_dyn(int id, const char* name) {
  event_t *event_ref = malloc(sizeof(event_t));
  event_t event = create_event(id, name);
  memcpy(event_ref, &event, sizeof(event_t));
  return event_ref;
}
