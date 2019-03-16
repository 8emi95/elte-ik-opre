#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

void handler(int signumber)
{
  signal(SIGTERM,SIG_DFL);
}

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
    char place[128];
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
void startEvent();



int main(){
	loadData();
	int option;
	do{
		printf("\n");
		printf("KING OF STANDS\n");
		printf("0: Exit\n");
		printf("1: Add guest\n");
		printf("2: Edit guest\n");
		printf("3: Delete guest\n");
		printf("4: Create event\n");
		printf("5: Delete event\n");
		printf("6: List guests\n");
		printf("7: Start event\n");
		printf("\n");
		scanf("%i", &option);
		system("clear");
		switch(option){
		
		case 0 : 
			saveData();
			break;
		case 1 :
			printf("Adding a new guest...\n");
			addGuest();
			break;
		case 2:
			printf("Modifying an existing guest...\n");
			printf("Guest ID: ");
			int modif;
			scanf("%i",&modif);
			modifyGuest(modif);
			break;
		case 3:
			printf("Deleting an existing guest...\n");
			printf("Guest ID: ");
			int del;
			scanf("%i",&del);
			deleteGuest(del);
			break;	
		case 4:
			printf("Adding a new event...\n");
			addEvent();
			break;
		case 5:
			printf("Deleting an existing event...\n");
			deleteEvent();
			break;
		case 6:
			listGuests();
			break;
		case 7:
			printf("Starting an event...\n");
			startEvent();
			break;
		default:
			
			printf("Please enter a number between 1 and 6.\n");
		}
	}while(option != 0);
    return 0;
}
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
		printf("Place: ");
		scanf("%s", &events[event_number].place);
		printf("Event ID is: %d \n", event_number);
		++event_number;
		saveData();
}

void deleteEvent(){
    printf("Event ID: ");
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
	FILE* guest_iofile = fopen("guest_iofile.txt", "r");
	fscanf(guest_iofile, "%d", &guest_number);
	int i;
	for(i = 0; i < guest_number; ++i){
		fscanf(guest_iofile, "%d %s %s %s %d", &guests[i].id, &guests[i].name, &guests[i].email, &guests[i].regtime, &guests[i].event_id);
	}
    fclose(guest_iofile);
    
	FILE* event_iofile = fopen("event_iofile.txt", "r");
    fscanf(event_iofile, "%d", &event_number);
    for(i = 0; i < event_number; ++i){
        fscanf(event_iofile, "%d %s %s", &events[i].id, &events[i].name, &events[i].place);
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
        fprintf(event_iofile, "%d %s %s\n", events[i].id, events[i].name, events[i].place);
    }
	
    fclose(event_iofile);
    fclose(guest_iofile);
}

void startEvent(){
	const char * pipe_filename = "pipe_guest_list.txt";	// csővezetékhez (vendéglista) használt fájl neve
	
	int pipe_event_place[2]; // csővezeték (helyszín)
    char pipe_string[100];  // karaktertömb csővezetékből való olvasáshoz
	
	pid_t child; // gyerekfolyamat ID
	
	// szignálkezelés
	signal(SIGTERM,handler);
	
	// csővezeték létrehozása
	if (pipe(pipe_event_place) == -1) 
	{
        perror("Hiba a csővezeték megnyitaskor!");
    }
	
	// csővezeték fájl létrehozása (vendéglista)
	int w;
	w = open(pipe_filename,O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(w < 0)
	{
		perror("\nHiba a fájl megnyitásakor. A program kilép.\n");
		exit(1);
	}
	close(w);
	
	// csővezeték fájl feltöltése (vendéglista)
	FILE * f;
	FILE * g;
	char t[1000];
	
	f = fopen("guest_iofile.txt","a+");
	g = fopen(pipe_filename,"a+");
	
	printf("ID of event to start:");
	int id;
	scanf("%d",&id);
	int k;
	fgets(t,sizeof(char)*1000,f);
	for(k = 0; k < guest_number; k++)
	{
		fgets(t,sizeof(char)*1000,f);
		if(guests[k].event_id == id){
			fputs(t,g);
		}
	}
	fclose(f);
	fclose(g);
	
	// FIFO fájl létrehozása
	mkfifo(pipe_filename, S_IRUSR | S_IWUSR );
	
	
	child = fork();
	
	if(child > 0 )	//központ
	{	
		// helyszín elküldése csővezetéken
		printf("King Of Stands: asking the partner to start event with ID: %d(%s) at %s...\n",id,events[id].name, events[id].place);
		close(pipe_event_place[0]);
		write(pipe_event_place[1],events[id].place,strlen(events[id].place));
        close(pipe_event_place[1]);
		pause();
		
		// vendéglista elküldése csővezetéken
		printf("King Of Stands: sending guestlist to the partner...\n");
		sleep(1);
		kill(child,SIGTERM); // jelzés, hogy a gyerek olvashatja a FIFO fájlt
		
		int status;
		wait(&status);
		
	}else	//partner
	{
		FILE * p;
		char s[1024];
		
		// helyszín kiolvasása csővezetékből
		close(pipe_event_place[1]);
		sleep(1);
		read(pipe_event_place[0],pipe_string,strlen(events[id].place));
		printf("Partner: received the event's location:  %s, and is on the way...\n",pipe_string);
		sleep(4);
		
		printf("Partner: arrived to the location, ready to host...\n");
		sleep(3);
		kill(getppid(),SIGTERM);
		pause();
		
		// csővezeték fájl olvasása (vendéglista)
		printf("Partner: received the following guestlist:\n");
		p = fopen(pipe_filename,"a+");
		while(fgets(s,sizeof(char)*1024,p))
		{
			printf("%s", s);
		}        
		fclose(p);
		sleep(5);
		
		// rendezvény sikeressége
		srand(time(NULL));
		int r = rand() % 100;
		printf("Partner: the event got %i points out of 100.\n",r);
		sleep(2);
		printf("Partner: the following guests missed the event:\n");
		int a = 0;
		p = fopen(pipe_filename,"a+");
		while(fgets(s,sizeof(char)*1024,p))
		{
			a++;
			srand(time(NULL) + a);
			int r2 = rand() % 10;
			if(r2 == 1)
			{
				printf("%s", s);
			}
		}
		fclose(p);
		
        unlink(pipe_filename); // csővezeték fájl törlése
	}
}


