#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


//  Tablaszerkezetek
// Guests(id; name; email; reg_time)
// Events(id, name)
struct Guest
{
   char id[128];
   char name[128];
   char email[128];
   char event_id[128];
   char reg_time[128];
   
};

struct Event
{
    char id[128];
    char name[128];
};

struct Guest guests[10000];
struct Event events[10000];

int guest_number = 0;
int event_num = 0;

void listGuests();
void addGuest();
void modifyGuest();
void deleteGuest(int a);
void addEvent();
void deleteEvent();
void loadData();
void saveData();

void listGuests(){
	
    printf("List of guests: \n");
	int i ;
    for(i = 0; i < guest_number; i++){
        printf("%s %s %s %s\n", guests[i].id, guests[i].name, guests[i].email, guests[i].reg_time);
    }
    
}

void addGuest(){
		++guest_number;
		sprintf(guests[guest_number].id, "%d", guest_number);
        printf("Name: ");
        scanf("%s", guests[guest_number].name);
        printf("E-mail: ");
        scanf("%s", guests[guest_number].email);
        printf("Event ID: ");
        scanf("%d", guests[guest_number].event_id);
        time_t rawtime;
        struct tm * timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        sprintf(guests[guest_number].reg_time, "%d-%d-%d",
        timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);
        printf("%s \n", guests[guest_number ].id);
}

void modifyGuest(){
    printf("Latogato modositasa..\n");
    deleteGuest(1);

}

void deleteGuest(int a){

    printf("Guest ID to delete: ");
    int i;
    scanf("%i", &i);
    guests[i] = guests[guest_number];
	guest_number--;
}

void addEvent(){
        printf("Adding new event..\n");
        if(event_num >= 1000){
            printf("Nem lehet tobb esemenyt felvenni a listara..\n");
        }
        else{
            // Feltesszuk, hogy az adott esemeny meg nem lett letrehozva
            ++event_num;
            sprintf(events[event_num - 2].id, "%d", (event_num - 2));
            scanf("%s", events[event_num - 2].name);
        }
        
}

void deleteEvent(){
    printf("Esemeny torlese.\n");
    printf("Adjon meg a listabol egy esemenyhez tartozo id\n");
    int i;
    for(i = 0; i < event_num; ++i){
        printf("%s %s", events[i].id, events[i].name);
    }
    scanf("%i", &i);
    events[i] = events[event_num--];
    
}


void loadData(){
	FILE* guest_iofile = fopen("guest_iofile.txt", "r");
	int i;
    // Reading guest list
	fscanf(guest_iofile, "%i", &guest_number);
	for(i = 0; i < guest_number; ++i){
		fscanf(guest_iofile, "%s %s %s %s", guests[i].id, guests[i].name, guests[i].email, guests[i].reg_time);
	}
    fclose(guest_iofile);
    
    // Reading event list
	FILE* event_iofile = fopen("event_iofile.txt", "r");
    fscanf(event_iofile, "%i", &event_num);
    for(i = 0; i < event_num; ++i){
        fscanf(event_iofile, "%s %s", events[i].id, events[i].name);
    }
    fclose(event_iofile);
}

void saveData(){

    FILE* guest_iofile = fopen("guest_iofile.txt", "w");
	FILE* event_iofile = fopen("event_iofile.txt", "w");
	int i;
	
    fprintf(guest_iofile, "%d\n", guest_number );
	
    for(i = 0; i < guest_number; ++i){
        fprintf(guest_iofile, "%s %s %s %s\n", guests[i].id, guests[i].name, guests[i].email, guests[i].reg_time);
    }
    
    fprintf(event_iofile, "%d\n", event_num );
	
    for(i = 0; i < event_num; ++i){
        fprintf(event_iofile, "%s %s\n", events[i].id, events[i].name);
    }
	
    fclose(event_iofile);
    fclose(guest_iofile);
}

int main(){
	loadData();
	int option;
	do{
		printf("0: Lists guests\n");
		printf("1: Add guest\n");
		printf("2: Edit guest\n");
		printf("3: Delete guest\n");
		printf("4: Create event\n");
		printf("5: Delete event\n");
		printf("6: Exit\n");
		scanf("%i", &option);
		switch(option){
		
		case 0 : 
			listGuests();
			break;
		case 1 :
			addGuest();
			break;
		case 2:
			modifyGuest();
			break;
		case 3:
			deleteGuest(0);
			break;	
		case 4:
			addEvent();
			break;
		case 5:
			deleteEvent();
			break;
		case 6:
			saveData();
			break;
		default:
			
			printf("Please enter a number between 1 and 6.\n");
		}
	}while(option != 6);
    return 0;
}



