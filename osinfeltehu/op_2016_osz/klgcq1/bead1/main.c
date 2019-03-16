#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>



struct Guest
{
   int id;
   char name[128];
   char email[128];
   int event_id;
   char regtime[128];
};


struct Event
{
    int id;
    char name[128];
};


struct Guest guests[10000];
struct Event events[10000];

int guest_number = 0;
int event_number = 0;

void listGuests();
void addGuest();
void modifyGuest(int id);
void deleteGuest(int id);
void addEvent();
void deleteEvent();
void loadData();	
void saveData();

void listGuests(){
	
    printf("List of guests: \n");
	int i ;
    for(i = 0; i < guest_number; i++){
        printf("ID: %d Name: %s E-mail: %s Registered: %s Event ID: %d \n", guests[i].id, guests[i].name, guests[i].email, guests[i].regtime, guests[i].event_id);
    }
    
}


void addGuest(){
		
		guests[guest_number].id = guest_number;
        printf("Name: ");
        scanf("%s", &guests[guest_number].name);
        printf("E-mail: ");
        scanf("%s", &guests[guest_number].email);
        printf("Event ID: ");
        scanf("%d", &guests[guest_number].event_id);
        time_t rawtime;
        struct tm * timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        sprintf(guests[guest_number].regtime, "%d-%d-%d",
        timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);
		printf("Guest ID is: %d \n", guest_number);
		++guest_number;
		saveData();
}

void modifyGuest(int id){
    
	if(id >= guest_number-1) {
		printf("Error: No guest found with the given ID.\n");
	} else {
        printf("Name: ");
        scanf("%s", &guests[id].name);
        printf("E-mail: ");
        scanf("%s", &guests[id].email);
        printf("Event ID: ");
        scanf("%d", &guests[id].event_id);
		saveData();
	}

}

void deleteGuest(int id){

	if(id >= guest_number) {
		printf("Error: No guest found with the given ID.\n");
	} else {
	printf("Guest with ID: %d sucessfully deleted.", id );
	if(id != guest_number - 1){
		printf("Note: guest with ID: %d gets the new ID: %d.\n", guest_number-1, id);
		guests[id] = guests[guest_number-1];
		guests[id].id = id;
	}
	guest_number--;
	saveData();
	}
}

void addEvent(){
		events[event_number].id = event_number;
        printf("Name: ");
        scanf("%s", &events[event_number].name);
		printf("Event ID is: %d \n", event_number);
		++event_number;
		saveData();
}

void deleteEvent(){
    printf("Event ID to delete: ");
    int i;
	scanf("%i",&i);
	if(i >= event_number-1) {
		printf("Error: No event found with the given ID.\n");
	} else {
	int j;
	for(j = guest_number-1; j >= 0 ; j-- ){
		if(guests[j].event_id == i){
			deleteGuest(j);
		}
	}
	printf("Event sucessfully deleted.");
	if(i != event_number - 1){
		events[i] = events[event_number - 1];
		printf("Note: event with ID: %d gets the new ID: %d.\n", event_number-1, i);
	}
	event_number--;
	saveData();
	}
    
}


void loadData(){
	printf("start");
	FILE* guest_iofile = fopen("guest_iofile.txt", "r");
    // Reading guest list
	fscanf(guest_iofile, "%d", &guest_number);
	int i;
	for(i = 0; i < guest_number; ++i){
		fscanf(guest_iofile, "%d %s %s %s %d", &guests[i].id, &guests[i].name, &guests[i].email, &guests[i].regtime, &guests[i].event_id);
	}
    fclose(guest_iofile);
    
    // Reading event list
	FILE* event_iofile = fopen("event_iofile.txt", "r");
    fscanf(event_iofile, "%d", &event_number);
    for(i = 0; i < event_number; ++i){
        fscanf(event_iofile, "%d %s", &events[i].id, &events[i].name);
    }
    fclose(event_iofile);
}

void saveData(){

    FILE* guest_iofile = fopen("guest_iofile.txt", "w");
	FILE* event_iofile = fopen("event_iofile.txt", "w");
	int i;
	
    fprintf(guest_iofile, "%d\n", guest_number );
	
    for(i = 0; i < guest_number; i++){
        fprintf(guest_iofile, "%d %s %s %s %d \n", guests[i].id, guests[i].name, guests[i].email, guests[i].regtime, guests[i].event_id);
    }
    
    fprintf(event_iofile, "%d\n", event_number );
	
    for(i = 0; i < event_number; ++i){
        fprintf(event_iofile, "%d %s\n", events[i].id, events[i].name);
    }
	
    fclose(event_iofile);
    fclose(guest_iofile);
}



int main(){
	loadData();
	int option;
	do{
		printf("0: Exit\n");
		printf("1: Add guest\n");
		printf("2: Edit guest\n");
		printf("3: Delete guest\n");
		printf("4: Create event\n");
		printf("5: Delete event\n");
		printf("6: List guests\n");
		printf("7: Start event\n");
		scanf("%i", &option);
		switch(option){
		
		case 0 : 
			saveData();
			break;
		case 1 :
			addGuest();
			break;
		case 2:
			printf("Guest ID: ");
			int modif;
			scanf("%i",&modif);
			modifyGuest(modif);
			break;
		case 3:
			printf("Guest ID: ");
			int del;
			scanf("%i",&del);
			deleteGuest(del);
			break;	
		case 4:
			addEvent();
			break;
		case 5:
			deleteEvent();
			break;
		case 6:
			listGuests();
			break;
		case 7:
			//startEvent();
			printf("start");
			break;
		default:
			
			printf("Please enter a number between 1 and 6.\n");
		}
	}while(option != 0);
    return 0;
}



