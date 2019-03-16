#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

struct Data {
	char name[30];
	char email[30];
	int eventid;
	char date[20];
};

struct Event {
	int id;
	char name[20];
};

void menu();
void insertEventFile();
void insert();
void list();
void modify();
void delete();
void listEvent();
void insertEvent();
void deleteEvent();

char* actualTime();

int main(int argc, char *argv[])
{	
	insertEventFile();
	printf("Eloszor egy rendezvenyt kell letrehoznia!\n");
	insertEvent();
	menu();	
	return 0;
}

void menu()
{
  char menu[2];
  while(1){
    printf("\n==========\nMENU\n==========\n");
    printf("Jelentkezes:\tj\nListazas:\tl\nModositas:\tm\nTorles:\t\tt\nRendezvenyek:\tr\nRendezveny felvetele:\tf\nRendezveny torlese:\td\nKilépés:\tCtrl+C\n");
    printf("Valasztott betu: ");
	scanf("%s", menu);
    if ( strcmp(menu, "j") == 0 )
		insert();
	else if(strcmp(menu, "l") == 0)
		list();
    else if ( strcmp(menu, "m") == 0 )
		modify();
    else if ( strcmp(menu, "t") == 0 )
		delete();
	else if (strcmp(menu, "r") == 0)
		listEvent();
	else if (strcmp(menu, "f") == 0)
		insertEvent();
	else if (strcmp(menu, "d") == 0)
		deleteEvent();
  }
}

void insertEventFile()
{
	int eventfile;
	eventfile = open("eventfile", O_RDONLY | O_WRONLY | O_CREAT | O_APPEND, 0700);
	close(eventfile);	
}

void insert()
{
	printf("Uj tag beszurasa.");
	
	int members;
	members = open("members", O_WRONLY | O_CREAT | O_APPEND);
	
	struct Data newData;
	struct Data allData;
	struct Event eventData;
		
	printf("\nAdd meg a nevedet: ");
	scanf("%s", &newData.name);
	printf("\nAdd meg az e-mail címedet: ");
	scanf("%s", &newData.email);
	printf("\nAdd meg a rendezvény azonosítóját: ");
	scanf("%d", &newData.eventid);
	
	int event = open("eventfile", O_RDONLY);
    if(event < 0) { perror("Error"); exit(1); }
	
    int number = 0;
    while (read(event, &eventData, sizeof(eventData)))
    {
			if(newData.eventid == eventData.id)
            number++;
	}
    close(event);	
	if(number == 0){ perror("Error"); exit(1); }
	
	char* tmp = actualTime();
	strcpy(&newData.date, tmp);
	
	printf("%s %s %d %s\n", &newData.name, &newData.email, newData.eventid, &newData.date);
	write(members,&newData,sizeof(newData));	
	close(members);

	int f = open("members", O_RDONLY);
	int c = 0;
	while (read(f, &allData, sizeof(allData)))
	{
		if(newData.eventid == allData.eventid)
			c++;
	}
	close(members);
	printf("Sikeres jelentkezes a %i. helyen.", c);
}

void list()
{	
	struct Data newData;
	
	int f = open("members", O_RDONLY);
	if(f < 0) { perror("Error"); exit(1); }
	
	while (read(f, &newData, sizeof(newData)))
	{
		printf("%s %s %d %s\n", &newData.name, &newData.email, newData.eventid, &newData.date);
	}
	close(f);
}

void modify()
{
	int f = open("members", O_RDONLY);
	if(f < 0) { perror("Error"); exit(1); }
	int g = open("tmp", O_WRONLY | O_CREAT | O_APPEND, 0700);
	if(g < 0) { perror("Error"); exit(1); }
	
	struct Data newData;
	struct Data oldData;
	struct Event eventData;
	
	printf("\nAdd meg a nevet, ahol modositani szeretnel: ");
	scanf("%s", &newData.name);
	printf("\nAdd meg az e-mail címedet: ");
	scanf("%s", &newData.email);
	printf("\nAdd meg a rendezvény azonosítóját: ");
	scanf("%d", &newData.eventid);
	
	int event = open("eventfile", O_RDONLY);
    if(event < 0) { perror("Error"); exit(1); }
	int number = 0;
    while (read(event, &eventData, sizeof(eventData)))
    {
			if(newData.eventid == eventData.id)
            number++;
	}
    close(event);	
	if(number == 0){ perror("Error"); exit(1); }
	
	char* time = actualTime();
	strcpy(&newData.date, time);	
	
	while(read(f, &oldData, sizeof(oldData)))
	{
		if(strcmp(oldData.name, newData.name))
		{
			write(g, &oldData, sizeof(oldData));			
		}
		
	}
	write(g,&newData,sizeof(newData));	
 
	close(f);	
	close(g);
	
	remove("members");
	rename("tmp", "members");
}

void delete()
{
	struct Data data;
	char tmpname[30];
	
	printf("Add meg a nevet, akit torolni szeretnel: ");
	scanf("%s", tmpname);
	
	int f = open("members", O_RDONLY);
	if(f < 0) { perror("Error"); exit(1); }
	
	int g = open("tmp", O_WRONLY | O_CREAT | O_APPEND, 0700);
	if(g < 0) { perror("Error"); exit(1); }
	
	while (read(f, &data, sizeof(data)))
	{
		if(strcmp(tmpname, data.name))
			write(g, &data, sizeof(data));		
	}	
	close(f);
	close(g);
	
	remove("members");
	rename("tmp", "members");
}


void listEvent()
{
	struct Event event;
	
	int f = open("eventfile", O_RDONLY);
	if (f<0) { perror("Error"); exit(1); }
	
	while (read(f, &event, sizeof(event)))
	{
		printf("%i %s\n", event.id, &event.name);
	}
	close(f);
}

void insertEvent()
{
	printf("Uj rendezveny beszurasa.");
	
	int eventfile;
	eventfile = open("eventfile", O_RDONLY | O_WRONLY | O_APPEND);
	struct Event newEvent;
		
	printf("\nAdd meg a rendezveny azonositojat: ");
	scanf("%d", &newEvent.id);
	printf("\nAdd meg a rendezveny nevet: ");
	scanf("%s", &newEvent.name);	
	
	write(eventfile,&newEvent,sizeof(newEvent));
	printf("\nA felvetel sikeres.");	
	close(eventfile);	
}

void deleteEvent()
{
	struct Data data;
	struct Event delete;
    struct Event oldEvent;	
	
	int e = open("eventfile", O_RDONLY);
    if(e < 0) { perror("Error"); exit(1); }
		
    int f = open("etmp", O_RDWR| O_CREAT | O_APPEND, 0700);
    if(f < 0) { perror("Error"); exit(1); }
   
    printf("\nAdd meg annak a rendezvenynek az azonositojat, amit torolni szeretnel: ");
    scanf("%d", &delete.id);
   
    while(read(e, &oldEvent, sizeof(oldEvent)))
    {
        if(delete.id != oldEvent.id)
			write(f,&oldEvent,sizeof(oldEvent));
    }
 
    close(e);
    close(f);
   
    int members = open("members", O_RDONLY);
    if(members < 0) { perror("Error"); exit(1); }
	
    int g = open("mtmp", O_RDWR| O_CREAT | O_APPEND, 0700);
    if(g < 0) { perror("Error"); exit(1); }
	
	 while(read(members, &data, sizeof(data)))
    {
        if(delete.id != data.eventid){		
            write(g,&data,sizeof(data));
        }
    }
   
    close(members);
	close(g);
   
    remove("eventfile");
    rename("etmp", "eventfile");
	remove("members");
	rename("mtmp", "members");
}


char* actualTime()
{  
	time_t current_time = time(NULL);
	struct tm * time_info;
	int const dateLen = strlen("xxxx.xx.xx xx:xx:xx") + 1;
	const char* const timeFormat = "%Y.%m.%d %H:%M:%S";
	char* x = (char*)malloc(sizeof(char) * dateLen);

	time_info = localtime(&current_time);
	strftime(x, dateLen, timeFormat, time_info);
	
	return x;
}
