#ifndef HELPER_FUNCTIONS_H_INCLUDED
#define HELPER_FUNCTIONS_H_INCLUDED

typedef struct _Guest {
	int id;
	char name[50];
	char email[50];
	char time[20];
} Guest;

typedef struct _GuestListElement {
	Guest *data;
	struct _GuestListElement *next;
} GuestListElement;

typedef struct _GuestList {
	int numberOfGuests;
	
	GuestListElement *head;
	GuestListElement *tail;
} GuestList;

typedef struct _Event {
	int id;
	char name[50];
} Event;

typedef struct _EventListElement {
	Event *data;
	struct _EventListElement *next;
} EventListElement;

typedef struct _EventList {
	EventListElement *head;
	EventListElement *tail;
} EventList;

typedef struct _EventResult {
	int successRate;
	int numberOfMissingGuests;
	char missingGuests[1024];
} EventResult;

void get_formated_time(char *ts);
void separator();
int getRandomValue(int maxValue);

#endif