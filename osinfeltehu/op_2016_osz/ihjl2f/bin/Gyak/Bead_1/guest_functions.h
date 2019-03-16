#ifndef GUEST_FUNCTIONS_H_INCLUDED
#define GUEST_FUNCTIONS_H_INCLUDED

#include "helper_functions.h"

void select_event(Event* event);
void collect_guest_list(Event *event, GuestList* guest_list);
void show_guest_list();
void register_guest();
void modify_guest(int delete);
void delete_guest();

#endif