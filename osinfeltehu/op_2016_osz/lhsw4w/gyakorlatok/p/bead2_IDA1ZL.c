#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

struct Data {
	char name[30];
	char email[30];
	int eventid;
	char date[20];
};

struct name
{
	char n[30];
	struct name * next;
};

struct Event {
	int id;
	char name[20];
	char city[40];
};

void menu();
void insertEventFile();
void insert();
void list();
void modify();
void delete();
void listEvent();
void startEvent();
void insertEvent();
void deleteEvent();
void handler(int signumber)
{
  printf(" ",signumber);
}
char* actualTime();

int main(int argc, char *argv[])
{	
	if( access( "eventfile", F_OK ) == -1 ) {
		insertEventFile();
		printf("Elsokent rendezvenyt kell megadnia!\n");
		insertEvent();
	} 
	menu();	
	return 0;
}

void menu()
{
  char menu[2];
  while(1){
    printf("\n==========\nMENU\n==========\n");
    printf("Jelentkezes:\tj\nListazas:\tl\nModositas:\tm\nTorles:\t\tt\nRendezvenyek:\tr\nRendezveny inditasa:\ts\nRendezveny felvetele:\tf\nRendezveny torlese:\td\nKilépés:\tCtrl+C\n");
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
	else if(strcmp(menu, "s") == 0)
		startEvent();
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
	strcpy(newData.date, tmp);
	
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
	close(f);
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
	scanf("%s", newData.name);
	printf("\nAdd meg az e-mail címedet: ");
	scanf("%s", newData.email);
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
	strcpy(newData.date, time);	
	
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
		printf("%i %s %s\n", event.id, &event.name, &event.city);
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
	printf("\nAdd meg a rendezveny helyszinet: ");
	scanf("%s", &newEvent.city);
	
	write(eventfile,&newEvent,sizeof(newEvent));
	printf("\nA felvetel sikeres.");	
	close(eventfile);	
}

void startEvent()
{
	struct Data data;
	struct Event eventData;
	int id;
	
	printf("Rendezveny inditasa.");
	printf("\nAdd meg annak a rendezvenynek az azonositojat, amit el szeretnel inditani: ");
	scanf("%d", &id);	
	
	//random szám:
	srand(time(NULL)); 
	int r=rand()%11; 
	
	signal(SIGUSR1,handler);
	signal(SIGUSR2,handler);	
	
	int pipefd[2];
	int pipefd2[2];
    pid_t pid;	
	if (pipe(pipefd) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }
	
	if (pipe(pipefd2) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }
	
	pid = fork();	// creating parent-child processes
    if (pid == -1) 
	{
        perror("Fork hiba");
        exit(EXIT_FAILURE);
    }
	
	char city[40];
	struct name names;
	struct name* p = &names;
	
	int event = open("eventfile", O_RDONLY);
    if(event < 0) { perror("Error"); exit(1); }
	int number = 0;
    while (read(event, &eventData, sizeof(eventData)))
    {
			if(eventData.id == id)
			{
				number++;
				strcpy(city, eventData.city);
			}
	}
    close(event);	
	if(number == 0){ perror("Error"); exit(1); }
	
	int member = open("members", O_RDONLY);
    if(member < 0) { perror("Error"); exit(1); }
	number = 0;
    while (read(member, &data, sizeof(data)))
    {
			if(data.eventid == id)
			{
				number++;
				strcpy (p->n,data.name);
				p->next = malloc(sizeof(struct name));
				p = p->next;
			}			
	}
	close(member);	
	if(number == 0){ perror("Error"); exit(1); }
	//realloc(p,sizeof(struct name));	
	
	char sz[40];
	//char c[30];
	struct name n;
	struct name notcome;
	struct name* q = &notcome;	
	struct name m;
	if(pid == 0)  //child
	{
		//printf("\ngyerek indul");
		read(pipefd[0], sz, sizeof(sz));
		close(pipefd[0]);
		printf("\nA rendezveny helyszine: %s", sz);
		printf("\nVarom a resztvevoket!");
		read(pipefd2[0], &n, sizeof(struct name));
		close(pipefd2[0]);
		struct name * a = &n;
		int randnum;
		while (a->next != NULL)
		{
			if(a->next != NULL)
			{
				//srand(time(NULL)); 
				randnum=rand()%101;
				printf("\nRandom szam:%d", randnum);
			}
			if(randnum <= 90)
			{
				if(a->next != NULL)
					printf("\nJelentkezett es megerkezett: %s", a -> n);
				a = a -> next;
			}
			else
			{
				if(a->next != NULL)
					printf("Nem erkezett meg a rendezvenyre: %s\n", a->n);
				strcpy(q->n, a->n);
				q->next = malloc(sizeof(struct name));
				q = q->next;
						
				a = a->next; 
			}			 
		}
		//sleep(1);
		write(pipefd[1], &r, sizeof(r));
		close(pipefd[1]);
		write(pipefd2[1], &notcome, sizeof(struct name));
		
		close(pipefd2[1]);
		printf("\ngyerek vege\n");		
		kill(getppid(), SIGUSR1);
		
	}	
	else		//parent
	{
		//printf("\nszulo indul");
		write(pipefd[1], city, sizeof(city));
		close(pipefd[1]);
		write(pipefd2[1], &names, sizeof(struct name));
		close(pipefd2[1]);
		printf("\nszulo beirta az adatokat");
		kill(pid, SIGUSR2);		
		pause();
		
		sleep(1);
		read(pipefd[0], &r, sizeof(r));
		printf("\nRendezveny sikeressege: %d", r);
		close(pipefd[0]);
		read(pipefd2[0], &m, sizeof(struct name));
		printf("\nAkik nem jottek el: ");
		struct name * s = &m;
	    while (s->next != NULL)
		{		
			if(s->next != NULL)	printf("%s\n", s->n);			
			s = s->next; 	
		}
		close(pipefd2[0]);
		printf("\nszulo vege\n");
	}
	exit(EXIT_SUCCESS);	
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
