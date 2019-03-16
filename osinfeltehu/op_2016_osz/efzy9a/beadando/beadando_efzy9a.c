/*
Akos Arva - EFZY9A
Tables:
	Guest(registration_number; name; email_address; event_id, subscription_date)
	Event(event_id; event_name; event_starts; event_ends)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Guest{
	char registration_number[50];
	char name[50];//07-11-16_00.00;
	char email_address[50];
	char event_id[50];
	char subscription_date[50];
};

struct Event{
	char event_id[50];
	char event_name[50];
	char event_starts[50];
	char event_ends[50];
};


int num_of_guests;
struct Guest* guests;
int num_of_events;
struct Event* events;

void guestSubscription(int a, int b);
void guestListing(int a);
void guestEdit();
void guestDelete();
void eventCreation();
void eventDelete();
void eventListing();
int loadDatabaseStatus(const char* event_list_file_name, const char* guest_list_file_name);
void saveDatabaseStatus();
void menu();

void guestSubscription(int a, int b){
	if(num_of_events > 0){
		// We suppose that the unchecked fields are appropiate
		if(a == 0){
			printf("Subscribe a guest for an event\n");
		}
		else{
			printf("Editing guest attributes.");
		}
		struct Guest new_guest;
		char* str_num_of_guests_new = malloc(sizeof(char));
		str_num_of_guests_new[0] = '0';
		if(a == 0){
			sprintf(str_num_of_guests_new, "%d", num_of_guests);
		}
		else{
			sprintf(str_num_of_guests_new, "%d", b);			
		}
		strcpy(new_guest.registration_number, str_num_of_guests_new);
		printf("Name: \n");
		scanf("%s", new_guest.name);
		printf("Email address: \n");
		scanf("%s", new_guest.email_address);
		printf("Event id\n");
		printf("Select an event id from the list below\n");
		eventListing();
		char* option = malloc(sizeof(char));
		int valid = 1;
		
		int i;
		do{
			scanf("%s", option);
			printf("%s", option);
			valid = 1;
			for(i = 0; i < num_of_events; ++i){
				if(0 == strcmp(option, events[i].event_id)){
					valid = 0;
				}
			}
			
			if(valid == 1){
				printf("Inappropiate number. Try again.\n");
			}
		}
		while(valid == 1);
		
		strcpy((char*)new_guest.event_id, (char*)option);
		
		// Getting system time
		time_t t = time(0);
		struct tm tm = *localtime(&t);
		char yrbf[50];
		char mnthbf[50];
		char dbf[50];
		
		
		sprintf(yrbf, "%d", tm.tm_year);
		sprintf(mnthbf, "%d", (tm.tm_mon + 1));
		sprintf(dbf, "%d", tm.tm_mday);
		
		strcat(new_guest.subscription_date, yrbf);
		strcat(new_guest.subscription_date, mnthbf);
		strcat(new_guest.subscription_date, dbf);
		
		guests = (struct Guest*)realloc(guests, (sizeof(struct Guest) *
			(num_of_guests + 1)));
		++num_of_guests;
		guests[num_of_guests - 1] = new_guest;
		printf("Guest application for the certain event successfully completed. %s\n", new_guest.registration_number - 1);
	}
	else{
		printf("There is no event, record an event before guest subscription.\n");
	}
	menu();
}

void guestListing(int a){
	printf("Guest listing\n");
	if(num_of_guests == 0){
		printf("The guest list is empty. Type 1 for recording new gurest..\n");
	}
	else{		
		// Guest(registration_number; name; email_address; event_id, subscription_date)
		printf("reg_number, name, email, event id, subscription date\n");
		int i;
		for(i = 0; i < num_of_guests; ++i){
			printf("%s %s %s %s %s\n", guests[i].registration_number,
				guests[i].name, guests[i].email_address,
				guests[i].event_id, guests[i].subscription_date);
		}
	}
	if(a == 0){
		menu();		
	}
}

void guestEdit(){
	printf("Editing guest.\n");
		guestListing(1);
		printf("Type that guest's registration number that you would like to edit.\n");
		char* option = malloc(sizeof(char));
		option[0] = '0';
		
		int exists;
		int b;
		do{
			scanf("%s", option);
			exists = 1;
			int i;
			for(i = 0; i < num_of_guests && exists == 1; ++i){
				if(0 == strcmp((char*)guests[i].registration_number, option)){
					exists = 0;
					b = i;
				}
			}
			
			if(0 == strcmp(option, "999")){// Cancel current operation
				exists = 2;
			}
			
			if(exists == 1){
				printf("The guest goes not exits. Try again or cancel : 999\n");
			}
		}
		while(exists == 1);
		
		if(exists == 0){
			struct Guest temporary;
			temporary = guests[num_of_guests - 1];
			guests[num_of_guests - 1] = guests[atoi(option)];
			guests[atoi(option)] = temporary;
			
			guests = (struct Guest*)realloc(guests, (sizeof(struct Guest) *
			(num_of_guests - 1)));
			
			--num_of_guests;
			
			
			guestSubscription(1, b);
		}
		menu();
}

void guestDelete(){
		printf("Deleting guest.\n");
		guestListing(1);
		printf("Type that guest's registration number that you would like to delete.\n");
		char* option = malloc(sizeof(char));
		option[0] = '0';
		
		int exists;
		do{
			scanf("%s", option);
			exists = 1;
			int i;
			for(i = 0; i < num_of_guests && exists == 1; ++i){
				if(0 == strcmp((char*)guests[i].registration_number, option)){
					exists = 0;
				}
			}
			
			if(0 == strcmp(option, "999")){// Cancel current operation
				exists = 2;
			}
			
			if(exists == 1){
				printf("The guest goes not exits. Try again or cancel : 999\n");
			}
		}
		while(exists == 1);
		
		if(exists == 0){
			struct Guest temporary;
			temporary = guests[num_of_guests - 1];
			guests[num_of_guests - 1] = guests[atoi(option)];
			guests[atoi(option)] = temporary;
			
			guests = (struct Guest*)realloc(guests, (sizeof(struct Guest) *
			(num_of_guests - 1)));
			
			--num_of_guests;
		}
		menu();
}

void eventCreation(){
	printf("Event creation.\n");
	
	printf("Type the name of the event(Maximum: 50ch).\n");
	// Suppose that the string contains maximum 50 characters.
	char* option = malloc(sizeof(char));
	option[0] = '0';
	int exists;
	do{
		scanf("%s", option);
		int i;
		exists = 1;
		for(i = 0; i < num_of_events && exists == 1; ++i){
			if(0 == strcmp(events[i].event_name, option)){
				exists = 0;
				break;
			}
		}
		
		if(0 == strcmp(option, "999")){
			exists = 2;// Quit
		}
		
		if(exists == 0){
			printf("The new event already exits. Try again. To cancel: 999\n");
		}
	}
	while(exists == 0);
	
	if(exists == 1){
		struct Event new_event;
		char* str_num_of_events_new = malloc(sizeof(char));
		str_num_of_events_new[0] = '0';
		sprintf(str_num_of_events_new, "%d", num_of_events);
		strcpy(new_event.event_id, str_num_of_events_new);
		strcpy(new_event.event_name, option);
		
		// Suppose that the date formats are valid
		printf("Enter the start time.\n");
		scanf("%s", new_event.event_starts);
		printf("Enter the end time.\n");
		scanf("%s", new_event.event_ends);
		events = (struct Event*)realloc(events, sizeof(struct Event) *
			(num_of_events + 1));
		++num_of_events;
		events[num_of_events - 1] = new_event;
	}
	menu();
}

void eventDelete(){
	printf("Event delete.\n");
	printf("Type the event id of the removable event.\n");
	eventListing();	
	char* option = malloc(sizeof(char));
	option[0] = '0';
	int exists;
	
	do{
		int i;
		scanf("%s", option);
		exists = 1;
		for(i = 0; i < num_of_events && exists == 1; ++i){
			if(0 == strcmp(option, (char*)events[i].event_id)){
				exists = 0;
			}
		}
		
		if(0 == strcmp(option, "999")){
			exists = 2;
		}
		
		if(exists == 1){
			printf("The event does not exist. Try again or cancel : 999\n");
		}
	}
	while(exists == 1);
	if(exists == 0){
		struct Event temporary = events[atoi(option)];
		events[atoi(option)] = events[num_of_events - 1];
		events[num_of_events - 1] = temporary;
		
		events = (struct Event*)realloc(events, sizeof(struct Event) *
			(num_of_events - 1));
		
		--num_of_events;		
	}
	menu();
}

void eventListing(){
	int i;
	printf("Event id, event name, event starts, event ends\n");
	for(i = 0; i < num_of_events; ++i){
		printf("%s %s %s %s\n", events[i].event_id, events[i].event_name,
			events[i].event_starts, events[i].event_ends);
	}
}

int loadDatabaseStatus(const char* event_list_file_name, const char* guest_list_file_name){
	printf("Loading database..\n");
	// Reading the database from the files(Guests, Events)
	FILE* event_file = fopen(event_list_file_name, "r");
	FILE* guest_file = fopen(guest_list_file_name, "r");
	if(event_file == NULL || guest_file == NULL){
		printf("One of the files of the two of the files  does/do not exist.\n");
		// Or another way is that the program creates the missing files.
		return 1;
	}
	else{
		// Reading event datas
		num_of_events = 0;
		fscanf(event_file, "%i\n", &num_of_events);
		// Event(event_id; event_name; event_starts; event_ends)
		if(num_of_events > 0){
			int i;
			events = (struct Event*)realloc(events, (sizeof(struct Event) *
				num_of_events));
			for(i = 0; i < num_of_events; ++i){
				fscanf(event_file, "%s %s %s %s\n", events[i].event_id,
					events[i].event_name, events[i].event_starts,
					events[i].event_ends);
			}
		}
		else{
			printf("The event list is empty(in this case the guest list too, mutually).\n");
		}
		
		// Reading guest datas
		num_of_guests = 0;
		fscanf(guest_file, "%i\n", &num_of_guests);
		// Guest(registration_number; name; email_address; event_id, subscription_date)
		if(num_of_guests > 0){
			int i;
			guests = (struct Guest*)realloc(guests, (sizeof(struct Guest) *
				num_of_guests));
			for(i = 0; i < num_of_guests; ++i){
				fscanf(guest_file, "%s %s %s %s %s\n",
					guests[i].registration_number, guests[i].name,
					guests[i].email_address, guests[i].event_id,
					guests[i].subscription_date);
			}
		}
		else{
			printf("The guest list is empty.\n");
		}
	}
	fclose(event_file);
	fclose(guest_file);
	return 0;
}

void saveDatabaseStatus(){
	FILE* event_file = fopen("event_list.txt", "w");
	FILE* guest_file = fopen("guest_list.txt", "w");
	// Updating event_file
	// Event(event_id; event_name; event_starts; event_ends)
	// Guest(registration_number; name; email_address; event_id, subscription_date)
	int i;
	char* str_num_of_events = malloc(sizeof(char));
	sprintf(str_num_of_events, "%d\n", num_of_events);
	//itoa(num_of_events, str_num_of_events, 10);
	//fputs(str_num_of_events, event_file);
	fprintf(event_file, "%s\n", str_num_of_events);
	for(i = 0; i < num_of_events; ++i){
		//char* line = (char*)malloc(sizeof(events[i].event_id) +
		//	sizeof(events[i].event_name) + sizeof(events[i].event_starts) +
		//	sizeof(events[i].event_ends));
		char str_event_id[50];
		//sprintf((char*)str_event_id, "%s", (char*events[i].event_id);
		//itoa(events[i].event_id, str_event_id, 10);
		//strcat(line, str_event_id);
		//strcat(line, " ");
		//strcat(line, events[i].event_name);
		//strcat(line, " ");
		//strcat(line, events[i].event_starts);
		//strcat(line, " ");
		//strcat(line, events[i].event_ends);
		//strcat(line, " ");
		//fputs(line, event_file);
		fprintf(guest_file, "%s %s %s %s\n", str_event_id, events[i].event_name, events[i].event_starts, events[i].event_ends);
	}
	//fflush(event_file);
	fclose(event_file);
	char* str_num_of_guests = malloc(sizeof(char));
	sprintf(str_num_of_guests, "%d\n", num_of_guests);
	//itoa(num_of_guests, str_num_of_guests, 10);
	//fputs(str_num_of_guests, guest_file);
	
	fprintf(guest_file, "%s\n", str_num_of_guests);
	
	for(i = 0; i < num_of_guests; ++i){
		//char* line = (char*)malloc(sizeof(guests[i].registration_number) +
		//	sizeof(guests[i].name) + sizeof(guests[i].email_address) +
		//	sizeof(guests[i].event_id) + sizeof(guests[i].subscription_date));
		char str_registration_number[50];
		sprintf(str_registration_number, "%s", guests[i].registration_number);
		//itoa(guests[i].registration_number, str_registration_number, 10);
		//strcat(line, str_registration_number);
		//strcat(line, " ");
		//strcat(line, guests[i].name);
		//strcat(line, " ");
		//strcat(line, guests[i].email_address);
		char str_event_id[50];
		sprintf(str_event_id, "%s", guests[i].event_id);
		//itoa(guests[i].event_id, str_event_id, 10);
		//strcat(line, " ");
		//strcat(line, str_event_id);
		//strcat(line, " ");
		//strcat(line, guests[i].subscription_date);
		//strcat(line, " ");
		//fputs(line, "%s\n");
		
		fprintf(guest_file, "%s %s %s %s %s\n", str_registration_number,
			guests[i].name, guests[i].email_address,
			str_event_id, guests[i].subscription_date);
	}
	//fflush(guest_file);
	fclose(guest_file);
}

void menu(){
	// Subscribe
	printf("1 - Sign a guest up for an event\n");
	// List by event filter or not
	printf("2 - List the subscribed guests for certain event\n");
	// Delete guest
	printf("3 - Delete a guest\n");
	// Start new event
	printf("4 - Create an event\n");
	// Delete event
	printf("5 - Delete an event\n");
	// Guest edit
	printf("6 - Guest edit\n");
	// Exit program
	printf("9 - Exit program\n");
	
	int valid = 1;
	int option;
	option = 0;
	do{
		printf("Type the number of the command\n");
		scanf("%i", &option);
		switch(option){
			case 1:
				valid = 0;
				break;
			case 2:
				valid = 0;
				break;
			case 3:
				valid = 0;
				break;
			case 4:
				valid = 0;
				break;
			case 5:
				valid = 0;
				break;
			case 6:
				valid = 0;
				break;
			case 9:
				valid = 0;
				break;
			default:
				printf("Inappropiate command number. Try again..\n");
				valid = 1;
		}
	}
	while(valid != 0);
	
	switch(option){
		case 1:
			guestSubscription(0, 999);
			break;
		case 2:
			guestListing(0);
			break;
		case 3:
			guestDelete();
			break;
		case 4:
			eventCreation();
			break;
		case 5:
			eventDelete();
			break;
		case 6:
			guestEdit();
			break;
		case 9:
			saveDatabaseStatus();
	}
}

int main(){
	printf("King of Stands\n");
	if(0 == loadDatabaseStatus("event_list.txt", "guest_list.txt")){
		menu();
	}
	return 0;
}