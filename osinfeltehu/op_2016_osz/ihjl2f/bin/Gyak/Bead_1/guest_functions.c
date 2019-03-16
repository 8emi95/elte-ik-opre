#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>

#include "helper_functions.h"

int select_event(Event* event) {
	printf("Please enter selected events numerical identifier:\n");
	char* event_id = NULL;
	size_t size;
	if (getline(&event_id, &size, stdin) != -1 ) {
		strtok(event_id, "\n");
		int selected_id = atoi(event_id);
		if (!get_event_by_id(selected_id, event)) {
			printf("Invalid event ID!\n");
			return 0;
		}
		
		return 1;
	}
	
	return 0;
}

void show_guest_list() {
	separator();
	
	// select event
	Event event;
	if (select_event(&event)) {
		printf("Selected event: %d - %s\n\n", event.id, event.name);
		
		// show guest list
		FILE *f;
		char file_name[sizeof(event.name) + sizeof(event.id) + 2];
		sprintf(file_name, "%d_%s", event.id, event.name);
		f = fopen(file_name, "rb");
		if (f==NULL){perror("Event file not found!\n"); return;}
		
		printf("Registered guests:\n");
		int tmp;
		fread(&tmp, sizeof(tmp), 1, f);
		while (!feof(f)){
			Guest current_guest;
			if (fread(&current_guest, sizeof(current_guest), 1, f)) {
				printf("%d - %s (%s) | %s\n", current_guest.id, current_guest.name, current_guest.email, current_guest.time);
			} else {
				break;
			}
		} 
		fclose(f);
	}
}

void register_guest() {
	separator();
	
	// select event
	Event event;
	if (select_event(&event)) {
		printf("Selected event: %d - %s\n", event.id, event.name);
	
		FILE *f;
		char file_name[sizeof(event.name) + sizeof(event.id) + 2];
		sprintf(file_name, "%d_%s", event.id, event.name);
		f = fopen(file_name, "r+b");
		if (f==NULL){perror("Event file not found!\n"); return;}
		
		int num;
		fread(&num, sizeof(num), 1, f);
		num = num + 1;
		
		// create guest
		size_t size;
		Guest new_guest;
		new_guest.id = num;
		printf("Enter guest name:\n");
		char* tmp = NULL;
		getline(&tmp, &size, stdin);
		strcpy(new_guest.name, tmp);
		strtok(new_guest.name, "\n");
		
		printf("Enter guets email address:\n");
		tmp = NULL;
		getline(&tmp, &size, stdin);
		strcpy(new_guest.email, tmp);
		strtok(new_guest.email, "\n");
		
		get_formated_time(tmp);
		strcpy(new_guest.time, tmp);
		
		// save guest
		rewind(f);
		fwrite(&num, sizeof(num), 1, f);
		fseek(f, 0, SEEK_END);
		fwrite(&new_guest, sizeof(new_guest), 1, f);
		
		fclose(f);
		
		printf("%s is the %d. registered guest!", new_guest.name, num);
		
	}
}

void modify_guest(int delete) {
	separator();
	
	// select event
	Event event;
	if (select_event(&event)) {
		printf("Selected event: %d - %s\n", event.id, event.name);
	
		FILE *f;
		char file_name[sizeof(event.name) + sizeof(event.id) + 2];
		sprintf(file_name, "%d_%s", event.id, event.name);
		f = fopen(file_name, "r+b");
		if (f==NULL){perror("Event file not found!\n"); return;}

		// select guest
		size_t size;
		Guest guest;
		printf("Please enter selected guests numerical identifier:\n");
		char* guest_id = NULL;
		getline(&guest_id, &size, stdin);
		strtok(guest_id, "\n");
		int selected_guest_id = atoi(guest_id);
			
		fseek(f, sizeof(int), SEEK_SET);
		long guest_first_byte = sizeof(int);
		int found = 0;
		while (!feof(f)){
			if (fread(&guest, sizeof(guest), 1, f)) {
				if (guest.id == selected_guest_id) {
					found = 1;
					break;
				}
				guest_first_byte = guest_first_byte + sizeof(guest);
			} else {
				break;
			}
		} 
		
		if (found) {
			printf("Selected guest: %d - %s (%s)\n", guest.id, guest.name, guest.email);
		} else {
			printf("Invalid guest ID!\n");
			return;
		}

		if (delete) {
			// delete guest
			 
			fseek(f, 0, SEEK_END);  
			long len = ftell(f);  
			char *ret = malloc(len - guest_first_byte - sizeof(Guest));  
			fseek(f, guest_first_byte + sizeof(Guest), SEEK_SET);  
			fread(ret, 1, len, f);

			fseek(f, guest_first_byte, SEEK_SET);
			fwrite(ret, len, 1, f);
		} else {
			// modify guest data
		
			int data_changed = 0;
			char* new_name = NULL;
			char* new_email = NULL;
			printf("New name: (leave blank to skip!)\n");
			getline(&new_name, &size, stdin);
			if (new_name[0] != '\n') {
				strtok(new_name, "\n");
				strcpy(guest.name, new_name);
				data_changed = 1;
			} else {
				printf("Name skipped!\n");
			}
			printf("New email: (leave blank to skip!)\n");
			getline(&new_email, &size, stdin);
			if (new_email[0] != '\n') {
				strtok(new_email, "\n");
				strcpy(guest.email, new_email);
				data_changed = 1;
			} else {
				printf("Email skipped!\n");
			}
			
			if (data_changed) {
				fseek(f, guest_first_byte, SEEK_SET);
				fwrite(&guest, sizeof(guest), 1, f);
				printf("Selected guest modified: %d - %s (%s)\n", guest.id, guest.name, guest.email);
			} else {
				printf("No data has been changed!\n");
			}
		}
		
		fclose(f);
	}
}

void collect_guest_list(Event *event, GuestList* guest_list) {
	FILE *f;
	char file_name[sizeof(event->name) + sizeof(event->id) + 2];
	sprintf(file_name, "%d_%s", event->id, event->name);
	f = fopen(file_name, "rb");
	if (f==NULL){perror("Event file not found!\n"); return;}
		
	fread(&guest_list->last_id, sizeof(int), 1, f);
	while (!feof(f)){
		GuestListElement *current = malloc(sizeof(GuestListElement));
		current->next = NULL;
		current->data = malloc(sizeof(Guest));
		if (fread(current->data, sizeof(Guest), 1, f)) {
			if (guest_list->head == NULL) {
				guest_list->head = current;
			}
			if (guest_list->tail != NULL) {
				guest_list->tail->next = current;
			}
			guest_list->tail = current;
			
			//printf("Debug guest: %d - %s (%s) | %s\n", guest_list->head->data->id, guest_list->head->data->name, guest_list->head->data->email, guest_list->head->data->time);
			//printf("Debug guest: %d - %s (%s) | %s\n", guest_list->tail->data->id, guest_list->tail->data->name, guest_list->tail->data->email, guest_list->tail->data->time);
		} else {
			break;
		}
	}
	
	fclose(f);	
}

void delete_guest() {
	separator();
	
	// select event
	Event event;
	if (select_event(&event)) {
		printf("Selected event: %d - %s\n", event.id, event.name);

		// select guest
		size_t size;
		printf("Please enter selected guests numerical identifier:\n");
		char* guest_id = NULL;
		getline(&guest_id, &size, stdin);
		strtok(guest_id, "\n");
		int selected_guest_id = atoi(guest_id);
		
		GuestList *guest_list = malloc(sizeof(GuestList));
		guest_list->head = NULL;
		guest_list->tail = NULL;
		
		collect_guest_list(&event, guest_list);
		int guest_deleted = 0;
		if (guest_list != NULL) {
			FILE *f;
			char file_name[sizeof(event.name) + sizeof(event.id) + 2];
			sprintf(file_name, "%d_%s", event.id, event.name);
			f = fopen(file_name, "wb");
			if (f==NULL){perror("Event file could not be opened!\n"); return;}
		
			fwrite((const void*) & guest_list->last_id, sizeof(int), 1, f);
			GuestListElement *current = guest_list->head;
			while (current != NULL) {
				if (current->data->id != selected_guest_id) {
					fwrite(current->data, sizeof(Guest), 1, f);
				} else {
					guest_deleted = 1;
				}
				current = current->next;
			}
			
			fclose(f);
		} 
		
		if (guest_deleted) {
			printf("Selected guest's registration has been revoked!\n");
		} else {
			printf("Could not find guest with this ID! Please check the guest list.\n");
		}
	}
}