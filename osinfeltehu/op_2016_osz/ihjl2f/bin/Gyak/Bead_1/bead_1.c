#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h>

const int number_of_options = 8;
const char available_avtions[] = {'1', '2', '3', '4', '5', '6', '7', 'q'};
const char process_file_name[] = "events_dictionary";
int next_event_id;

struct Event {
	int id;
	char name[50];
};

struct Guest {
	int id;
	char name[50];
	char email[50];
	char time[20];
};

void get_formated_time(char *ts) {
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime (ts,20,"%G-%m-%d %H:%M",timeinfo);
}

void new_event_added(struct Event* new_event) {
	next_event_id = next_event_id + 1;
	
	FILE *f = fopen( process_file_name, "r+b" );
	rewind(f);
	fwrite( &next_event_id, sizeof( next_event_id ), 1, f );
	fseek( f, 0, SEEK_END );
	fwrite(new_event, sizeof(struct Event), 1, f);
	fclose(f);
}

void separator() {
	printf("\n\n******************\n\n");
}

void show_main_menu() {
	separator();
	printf("Available actions:\n");
	printf("1. Event list\n");
	printf("2. New event\n");
	printf("3. Register guest\n");
	printf("4. Guest list\n");
	printf("5. Modify guest\n");
	printf("6. Delete registration\n");
	printf("7. Clear event\n");
	printf("\nq. Quit\n");
}

char main_menu() {
	show_main_menu();
	system("/bin/stty raw");
	while(1) {
		char selected = getchar();
		int index;
		for (index = 0; index < number_of_options; index = index + 1) {
			if (selected == available_avtions[index]) {
				system("/bin/stty cooked");
				return selected;
			}
		}
	}
}

void create_new_event() {
	separator();
	
	struct Event new_event;
	
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
		fwrite((const void*) & tmp, sizeof(next_event_id), 1, f);
		fclose(f);
		
		// update event dictionary
		new_event_added(&new_event);
	}
}

int get_event_by_id(int selected_id, struct Event* event) {
	int found = 0;
	FILE *f;
	f = fopen(process_file_name, "rb");
	if (f==NULL){perror("Initial process data could not be found!\n"); exit(1);}
	
	fread(&next_event_id, sizeof(next_event_id), 1, f);
	while (!feof(f)){
		struct Event current_event;
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

void register_guest() {
	separator();
	
	// select event
	struct Event event;
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
		struct Guest new_guest;
		new_guest.id = num;
		printf("Enter guets name:\n");
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
	struct Event event;
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
		
		printf("Selected event: %d - %s\n", event.id, event.name);
	
		FILE *f;
		char file_name[sizeof(event.name) + sizeof(event.id) + 2];
		sprintf(file_name, "%d_%s", event.id, event.name);
		f = fopen(file_name, "r+b");
		if (f==NULL){perror("Event file not found!\n"); return;}

		// select guest
		struct Guest guest;
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
			char *ret = malloc(len - guest_first_byte - sizeof(struct Guest));  
			fseek(f, guest_first_byte + sizeof(struct Guest), SEEK_SET);  
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

void show_event_list() {
	separator();
	FILE *f;
	f = fopen(process_file_name, "rb");
	if (f==NULL){perror("Initial process data could not be found!\n"); exit(1);}
	
	fread(&next_event_id, sizeof(next_event_id), 1, f);
	while (!feof(f)){
		struct Event current_event;
		if (fread(&current_event, sizeof(current_event), 1, f)) {
			printf("%d - %s", current_event.id, current_event.name);
		} else {
			break;
		}
	} 
	fclose(f);
}

void show_guest_list() {
	separator();
	
	// select event
	struct Event event;
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
		
		printf("Selected event: %d - %s\n", event.id, event.name);
		
		// show guest list
		FILE *f;
		char file_name[sizeof(event.name) + sizeof(event.id) + 2];
		sprintf(file_name, "%d_%s", event.id, event.name);
		f = fopen(file_name, "rb");
		if (f==NULL){perror("Event file not found!\n"); return;}
		
		int tmp;
		fread(&tmp, sizeof(tmp), 1, f);
		while (!feof(f)){
			struct Guest current_guest;
			if (fread(&current_guest, sizeof(current_guest), 1, f)) {
				printf("%d - %s (%s)\n", current_guest.id, current_guest.name, current_guest.email);
			} else {
				break;
			}
		} 
		fclose(f);
	}
}

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

int main(int argc,char** argv){
	init_process_file();
	
	char action;
	do {
		action = main_menu();
		switch (action) {
			case '1':
				show_event_list();
				break;
			case '2':
				create_new_event();
				break;
			case '3':
				register_guest();
				break;
			case '4':
				show_guest_list();
				break;
			case '5':
				modify_guest(0);
				break;
			case '6':
				modify_guest(1);
				break;
			default:
				break;
		}
	} while(action != 'q');
	
	printf("\n\nFerwell!\n\n");

	return 0;
}
