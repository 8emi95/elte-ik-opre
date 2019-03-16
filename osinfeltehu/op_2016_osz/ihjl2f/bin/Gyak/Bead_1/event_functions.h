#ifndef EVENT_FUNCTIONS_H_INCLUDED
#define EVENT_FUNCTIONS_H_INCLUDED

#include "helper_functions.h"

void init_process_file();
void show_event_list();
void new_event_added(Event* new_event);
int get_event_by_id(int selected_id, Event* event);
void create_new_event();
void reset_event();

#endif