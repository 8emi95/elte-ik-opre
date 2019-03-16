#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>

#include "helper_functions.h"

const char process_file_name[] = "events_dictionary";
int next_event_id;

void init_process_file() {
	FILE * f;
	f=fopen(process_file_name,"rb");
	if (f==NULL) {
		f=fopen(process_file_name,"wb");
		next_event_id = 1;
		fwrite((const void*) & next_event_id, sizeof(next_event_id), 1, f);
		fclose(f);
	} else {
		fread(&next_event_id, sizeof(next_event_id), 1, f);
		fclose(f);
	}
}

void show_event_list() {
	separator();
	FILE *f;
	f = fopen(process_file_name, "rb");
	if (f==NULL){perror("Initial process data could not be found!\n"); exit(1);}
	
	printf("Ongoing events:\n");
	fread(&next_event_id, sizeof(next_event_id), 1, f);
	while (!feof(f)){
		Event current_event;
		if (fread(&current_event, sizeof(current_event), 1, f)) {
			printf("%d - %s\n", current_event.id, current_event.name);
		} else {
			break;
		}
	} 
	fclose(f);
}

void new_event_added(Event* new_event) {
	next_event_id = next_event_id + 1;
	
	FILE *f = fopen( process_file_name, "r+b" );
	rewind(f);
	fwrite( &next_event_id, sizeof( next_event_id ), 1, f );
	fseek( f, 0, SEEK_END );
	fwrite(new_event, sizeof(Event), 1, f);
	fclose(f);
}

int get_event_by_id(int selected_id, Event* event) {
	int found = 0;
	FILE *f;
	f = fopen(process_file_name, "rb");
	if (f==NULL){perror("Initial process data could not be found!\n"); exit(1);}
	
	fread(&next_event_id, sizeof(next_event_id), 1, f);
	while (!feof(f)){
		Event current_event;
		if (fread(&current_event, sizeof(current_event), 1, f)) {
			if (current_event.id == selected_id) {
				found = 1;
				event->id = current_event.id;
				strcpy(event->name, current_event.name);
				break;
			}
		} else {
			break;
		}
	} 
	fclose(f);
	return found;
}

void create_new_event() {
	separator();
	
	Event new_event;
	
	printf("Please enter event name:\n");
	char* event_name = NULL;
	size_t size;
	if (getline(&event_name, &size, stdin) == -1 ) {
		printf("Invalid name!\n");
		return;
	} else {
		// Create new event
		strtok(event_name, "\n");
		new_event.id = next_event_id;
		strcpy(new_event.name, event_name);
		int tmp = 0;
	
		FILE * f;
		char file_name[sizeof(new_event.name) + sizeof(new_event.id) + 2];
		sprintf(file_name, "%d_%s", new_event.id, new_event.name);
		f=fopen(file_name,"wb");
		fwrite((const void*) & tmp, sizeof(tmp), 1, f);
		fclose(f);
		
		// update event dictionary
		new_event_added(&new_event);
	}
}

void reset_event() {
	separator();
	
	// select event
	Event event;
	printf("Please enter selected events numerical identifier:\n");
	char* event_id = NULL;
	size_t size;
	if (getline(&event_id, &size, stdin) != -1 ) {
		strtok(event_id, "\n");
		int selected_id = atoi(event_id);
		if (!get_event_by_id(selected_id, &event)) {
			printf("Invalid event ID!\n");
			return;
		}
		
		int tmp = 0;
		FILE * f;
		char file_name[sizeof(event.name) + sizeof(event.id) + 2];
		sprintf(file_name, "%d_%s", event.id, event.name);
		f=fopen(file_name,"wb");
		if (f==NULL){perror("Event file could not opened!\n"); return;}
		
		fwrite((const void*) & tmp, sizeof(tmp), 1, f);
		fclose(f);
	}
}