#ifndef HELPER_FUNCTIONS_H_INCLUDED
#define HELPER_FUNCTIONS_H_INCLUDED

typedef struct _Event {
	int id;
	char name[50];
} Event;

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
	int last_id;
	//int (*match)(const void *key1, const void *key2);
	//void (*destroy)(void *data);

	GuestListElement *head;
	GuestListElement *tail;
} GuestList;

void get_formated_time(char *ts);
void separator();

#endif