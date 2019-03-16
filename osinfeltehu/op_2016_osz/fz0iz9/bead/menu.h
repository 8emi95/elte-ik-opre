#include <stdio.h>
#include "vector.h"
#include "types.h"

#define MENU_CONTINUE 0
#define MENU_EXIT 1

#define MENU_INVALID 1
#define MENU_VALID 0

int main_menu(vector *events);
int guestlist_menu(Event* e);
static int get_event_index(vector* events);
static int get_guest_index(Event* event);
void flush_stdin();
void delete_event(vector* events);
void read_string(int min_length, char* question, char* target,int allowempty);
void add_event(vector * events);
void add_guest(Event * event);
void delete_guest(Event* event);
void edit_guest(Event* event);

