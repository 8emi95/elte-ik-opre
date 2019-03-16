#ifndef DATATYPES_H
#define DATATYPES_H

#include <time.h>

#define GUEST_NAME_LENGTH 	(size_t)(21)
#define EMAIL_LENGTH 		(size_t)(51)
#define EVENT_NAME_LENGTH 	(size_t)(51)
#define EVENT_PLACE_LENGTH	(size_t)(51)

typedef struct guest_data
{
	size_t 	id;
	char 	name[GUEST_NAME_LENGTH];	// strcpy()
	char 	email[EMAIL_LENGTH];		// strcpy()
	size_t	event_id;
	time_t 	reg_time;
} guest_data;

typedef struct guest_node
{
	struct guest_data guest;
	struct guest_node* next;
} guest_node;

typedef struct guest_list
{
	struct guest_node* head;
	struct guest_node* last;
} guest_list;

typedef struct event_data
{
	size_t id;
	char name[EVENT_NAME_LENGTH];	// strcpy()
	char place[EVENT_PLACE_LENGTH];	// strcpy()
} event_data;

typedef struct event_node
{
	struct event_data event;
	struct event_node* next;
} event_node;

typedef struct event_list
{
	struct event_node* head;
	struct event_node* last;
} event_list;

#endif //DATATYPES_H