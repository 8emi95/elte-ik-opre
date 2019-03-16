#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
 
struct Data{
    char name[100];
    char email[100];
    int eventid;
    char date[20];
};
 
struct Event {
    int id;
    char name[30];
	char city[30];
};

struct Name{
	char n[30];
	struct Name* next;
};
 
void menu();
void insert();
void list();
void insertEventFile();
void insertMemberFile();
void modify();
void delete();
void listEvent();
void insertEvent();
void deleteEvent();
void startEvent();
char* actualTime();

 void handler(int signumber){
  printf("",signumber);
}
 
int main(int argc, char *argv[])
{
	
	if( access( "eventfile", F_OK ) == -1 ) {
		insertEventFile();
		printf("Elsokent egy rendezvenyt kell megadni!\n");
		insertEvent();
		insertMemberFile();
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
    eventfile = open("eventfile", O_WRONLY|O_CREAT|O_APPEND, 0600);
    write(eventfile, '\0', 0);
    close(eventfile);  
}

void insertMemberFile(){
	int memberfile;
    memberfile = open("members", O_WRONLY|O_CREAT|O_APPEND, 0600);
    write(memberfile, '\0', 0);
    close(memberfile);  
}
 
void insert()
{
    printf("Uj tag beszurasa.");
		 
    struct Data newData;
    struct Data allData;
	struct Event eventData;

   
    int members;
    members = open("members", O_WRONLY | O_CREAT | O_APPEND);

	  
    printf("\nKerem adja meg a nevet: ");
    scanf("%s", newData.name);
    printf("\nKerem adja meg az e-mail cimet: ");
    scanf("%s", newData.email);
	
	//email formatum ellenorzese
	const char ch1 = '@';
	const char ch2 = '.';
	if( strchr(newData.email, ch1)==NULL || strchr(newData.email, ch2)==NULL ) { perror("Rossz az e-mail cim formatuma"); exit(1);}
	
    printf("\nKerem adja meg a rendezveny azonositojat: ");
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
   
    printf("%s %s %d %s\n", newData.name, newData.email, newData.eventid, newData.date);
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
         printf("%s %s %d %s\n", newData.name, newData.email, newData.eventid, newData.date);
    }
    close(f);
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

 
void modify()
{
	int f = open("members", O_RDONLY);
	if(f < 0) { perror("Error"); exit(1); }
	int g = open("tmp", O_WRONLY | O_CREAT | O_APPEND, 0600);
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
 
void listEvent()
{
    struct Event event;
   
    int f = open("eventfile", O_RDONLY);
    if (f<0) { perror("Error"); exit(1); }
   
    while (read(f, &event, sizeof(event)))
    {
        printf("%i %s %s\n", event.id, event.name, event.city);
    }
    close(f);
}
 
void insertEvent()
{
	struct Event newEvent;
	struct Event eventData;
	
    printf("Uj rendezveny beszurasa.");
   
    int eventfile;
    eventfile = open("eventfile", O_WRONLY | O_APPEND);
       
    printf("\nKerem adja meg a rendezveny azonositojat: ");
    scanf("%d", &newEvent.id);
	
	int event = open("eventfile", O_RDONLY);
    if(event < 0) { perror("Error"); exit(1); }	
    int number = 0;
    while (read(event, &eventData, sizeof(eventData)))
    {
			if(eventData.id == newEvent.id)
            number++;
	}
    close(event);
	if(number > 0){ perror("Mar van ilyen azonositoju esemeny\n"); exit(1); }
	
	
    printf("\nKerem adja meg a rendezveny nevet: ");
    scanf("%s", newEvent.name);
	printf("\nKerem adja meg a rendezveny varosat: ");
    scanf("%s", newEvent.city);
   
    printf("%d %s %s\n", newEvent.id, newEvent.name, newEvent.city);
    write(eventfile,&newEvent,sizeof(newEvent));   
    close(eventfile);  
}
 
 void startEvent()
{
	struct Event eventData;
	struct Data memberData;
	struct Data membersData;
	struct Name names;
	struct Name* p = &names;
	struct Name tempn;
	struct Name doesntcome;
	struct Name* q = &doesntcome;
	struct Name tempm;
	
	char city[30];
	char name[30];
	char sz[100];
	int id;
	int rate, random;
	int number = 0;
	
	srand(time(NULL)); 
	int r=rand()%11; 
	   
	printf("Kerem adja meg a rendezveny azonositojat!\n");
	scanf("%d", &id);

	int event = open("eventfile", O_RDONLY);
    if(event < 0) { perror("Error"); exit(1); }
    while (read(event, &eventData, sizeof(eventData)))
    {
			if(eventData.id == id) {
				number++;
				strcpy(city, eventData.city);
			}			
	}
    close(event);
	if(number == 0){ perror("Nincs ilyen rendezveny"); exit(1); }
	
	int member = open("members", O_RDONLY);
    if(member < 0) { perror("Error"); exit(1); }
	number = 0;
    while (read(member, &memberData, sizeof(memberData)))
    {
			if(memberData.eventid == id)
			{
				number++;
				strcpy (p->n,memberData.name);
				p->next = malloc(sizeof(struct Name));
				p = p->next;
			}
	}
    close(member);	
	if(number == 0){ perror("Error"); exit(1); }	
	   
	int pipefd1[2]; 
	int pipefd2[2];
	
    pid_t pid;
      
	signal(SIGUSR1,handler);
	signal(SIGUSR2,handler);

    if (pipe(pipefd1) == -1){
            perror("Hiba az 1-es pipe nyitaskor!");
            exit(EXIT_FAILURE);
       }
    if (pipe(pipefd2) == -1){
            perror("Hiba a 2-es pipe nyitaskor!");
            exit(EXIT_FAILURE);
       }
		   
    pid = fork();	
    if (pid == -1){
            perror("Fork hiba");
            exit(EXIT_FAILURE);
       }

    if (pid == 0)
	{// child process
                read(pipefd1[0],sz,sizeof(sz)); 
				close(pipefd1[0]);				
                printf("Megerkeztem a kovetkezo helyszinre: %s",sz);         
			    printf("\nVarom a vendegeket! \n");
				read(pipefd2[0], &tempn, sizeof(struct Name));
				close(pipefd2[0]);
				struct Name * a = &tempn;
				while (a->next != NULL)
				{
					if(a->next != NULL){
						random=rand()%101; 
						printf("%d\n", random);
					}
										
					if(random <= 90){
						if(a->next != NULL)	printf("Megerkezett a rendezvenyre: %s\n", a->n);			
						a = a->next; 
					}else{
						if(a->next != NULL)	printf("Nem erkezett meg a rendezvenyre: %s\n", a->n);
						
						strcpy(q->n, a->n);
						q->next = malloc(sizeof(struct Name));
						q = q->next;
						
						a = a->next; 
					}					
				}
				
		
								
				write(pipefd1[1], &r, sizeof(r));
				close(pipefd1[1]); 
				write(pipefd2[1], &doesntcome, sizeof(struct Name));
				close(pipefd2[1]); 
				printf("Gyerek befejezte!\n");
			    kill(getppid(),SIGUSR1);						
           }    	
           else 
    {// parent process          
               write(pipefd1[1], city,sizeof(city)); 
			   close(pipefd1[1]);
           //    printf("Szulo beirta a varosnevet a csobe!\n");

			   write(pipefd2[1], &names, sizeof(struct Name));
			   close(pipefd2[1]);
			//   printf("Szulo beirta a neveket\n");
			   kill(pid, SIGUSR2);
			   pause();
			   
			  sleep(1);
			  read(pipefd1[0],&rate,sizeof(rate)); 
              printf("Az esemeny sikeressege: %d\n", rate);
			  read(pipefd2[0], &tempm, sizeof(struct Name));
			  printf("Azoknak a nevei akik nem jöttek el: \n");
			  
			  struct Name * b = &tempm;
			  if(b->next == NULL){
				  printf("Nincs ilyen szemely\n");
			  }else{
			   while (b->next != NULL)
				{		
						if(b->next != NULL)
							printf("%s\n", b->n);			
						b = b->next; 	
				}
			  }		  			  
			  close(pipefd1[0]);
			  close(pipefd2[0]);
	       printf("Szulo befejezte!\n");	
	   }
	   exit(EXIT_SUCCESS);	// force exit, not necessary
}

 
void deleteEvent()
{
	struct Data data;
	struct Event delete;
    struct Event oldEvent;	
	
	int e = open("eventfile", O_RDONLY);
    if(e < 0) { perror("Error"); exit(1); }
		
    int f = open("etmp", O_RDWR| O_CREAT | O_APPEND, 0600);
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