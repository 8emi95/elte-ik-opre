#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
 
struct Data{
	int memberId;
    char name[100];
    char email[100];
    int eventId;
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
 
 void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}
 
void menu();
void login();
void insertEventFile();
void insertMemberFile();
void modify();
void list();
void deleteMember();
void listEvent();
void insertEvent();
void deleteEvent();
void startEvent();
char* timeStamp();
 
int main(int argc, char *argv[])
{
	
	if( access( "eventfile", F_OK ) == -1 ) {
		insertEventFile();
		printf("Elsokent adjon meg egy rendezvenyt!\n");
		insertEvent();
		insertMemberFile();
	} 
   
    menu();
    return 0;
}
 
void menu()
{
  int ch;
   do
   {  
      printf("\n== M E N U ==\n");
      printf("0. Kilepes\n");
      printf("1. Jelentkezes rendezvenyre\n");
      printf("2. Vendedek kiiratasa\n");
      printf("3. Vendeg adatainak modositasa\n");
      printf("4. Vendeg adatainak torlese\n");
      printf("5. Uj rendezveny\n");
      printf("6. Rendezveny kiiratasa\n");
      printf("7. Rendezveny adatainak torlese\n");
      printf("8. Rendezveny inditasa\n");
      printf("\n");
      printf("Valasztott menupont sorszama: ");
      scanf("%d", &ch);
   } while ( ch < 0 || ch > 8);
   while( getchar() != '\n' ) ;
 
   switch(ch)
   {
      case 1:
         login(); break;
      case 2:
         list(); break;
      case 3:
         modify(); break;
      case 4:
          deleteMember(); break;
      case 5:
          insertEvent(); break;
      case 6:
          listEvent(); break;
      case 7:
		deleteEvent(); break;
	  case 8:
		startEvent(); break;
   }
   
  if( ch ) menu();
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
	int r=rand()%101; 
	   
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
			if(memberData.eventId == id)
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
              printf("Az esemeny sikeressege: %d", rate);
			  printf("%\n");
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

 
void login()
{
    printf("Uj tag beszurasa.");
		 
    struct Data newData;
    struct Data allData;
	struct Data countData;
	struct Event eventData;
	
	int countmember = open("members", O_RDONLY);
    int count = 0;
    while (read(countmember, &countData, sizeof(countData))){ 
		if(count <= countData.memberId){
			count = countData.memberId; 
		}
	}
    close(countmember);
   
    int members;
    members = open("members", O_WRONLY | O_CREAT | O_APPEND);
	
	++count;
    //strcpy(newData.memberId, &count);
	newData.memberId = count;
	  
    printf("\nKerem adja meg a nevet: ");
    scanf("%s", newData.name);
    printf("\nKerem adja meg az e-mail cimet: ");
    scanf("%s", newData.email);
	
	//email formatum ellenorzese
	const char ch1 = '@';
	const char ch2 = '.';
	if( strchr(newData.email, ch1)==NULL || strchr(newData.email, ch2)==NULL ) { perror("Rossz az e-mail cim formatuma"); exit(1);}
	
    printf("\nKerem adja meg a rendezveny azonositojat: ");
    scanf("%d", &newData.eventId); 
	
    int event = open("eventfile", O_RDONLY);
    if(event < 0) { perror("Error"); exit(1); }
	
    int number = 0;
    while (read(event, &eventData, sizeof(eventData)))
    {
			if(newData.eventId == eventData.id)
            number++;
	}
    close(event);
	
	if(number == 0){ perror("Error"); exit(1); }
	
    char* tmp = timeStamp();
    strcpy(newData.date, tmp);
   
    printf("%d %s %s %d %s\n", newData.memberId, newData.name, newData.email, newData.eventId, newData.date);
    write(members,&newData,sizeof(newData));   
    close(members);
 
    int f = open("members", O_RDONLY);
    int c = 0;
    while (read(f, &allData, sizeof(allData)))
    {
        if(newData.eventId == allData.eventId)
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
         printf("%d %s %s %d %s\n", newData.memberId, newData.name, newData.email, newData.eventId, newData.date);
    }
    close(f);
}
 
void deleteMember()
{
    printf("Jelentkezett torlese.");
   
    int members = open("members", O_RDONLY);
    if(members < 0) { perror("Error"); exit(1); }
    int g;
    g = open("mas", O_RDWR| O_CREAT | O_APPEND, 0600);
    if(g < 0) { perror("Error"); exit(1); }
   
    struct Data deleteData;
    struct Data oldData;
   
    printf("\nKerem adja meg a jelentkezett azonositojat kit akar torolni: ");
    scanf("%d", &deleteData.memberId);
    while (read(members, &oldData, sizeof(oldData)))
    {
       // if(strcmp(oldData.name, deleteData.name) != 0){
		   if(oldData.memberId != deleteData.memberId){
          //  printf("%d %s %s %d %s\n", oldData.memberId, oldData.name, oldData.email, oldData.eventId, oldData.date);
           write(g,&oldData,sizeof(oldData));
        }
		 
    }
 
    close(members);
    close(g);
   
    remove("members");
    rename("mas", "members");
}
 
void modify(){
    printf("Jelentkezett adatainak modositasa.");
   
    int members = open("members", O_RDONLY);
    if(members < 0) { perror("Error"); exit(1); }
    int g;
    g = open("mas", O_RDWR| O_CREAT | O_APPEND, 0600);
    if(g < 0) { perror("Error"); exit(1); }
   
    struct Data modifyData;
    struct Data oldData;  
	struct Event eventData;	
   
    printf("\nKerem adja meg az azonositot akinel modositana: ");
    scanf("%d", &modifyData.memberId);
    printf("\nUj adatok megadasa:\n");
	 printf("\nKerem adja meg az uj nevet: ");
    scanf("%s", modifyData.name);
    printf("\nAdja meg az uj e-mail cimet: ");
    scanf("%s", modifyData.email);
    printf("\nAdja meg az uj rendezveny azonositot: ");
    scanf("%d", &modifyData.eventId);  
	
	int event = open("eventfile", O_RDONLY);
    if(event < 0) { perror("Error"); exit(1); }
	
    int number = 0;
    while (read(event, &eventData, sizeof(eventData)))
    {
			if(modifyData.eventId == eventData.id)
            number++;
	}
    close(event);
	
	if(number == 0){ perror("Error"); exit(1); }
	
    char* tmp = timeStamp();
    strcpy(modifyData.date, tmp);
   
    printf("%d %s %s %d %s\n", modifyData.memberId, modifyData.name, modifyData.email, modifyData.eventId, modifyData.date);
   
    while (read(members, &oldData, sizeof(oldData)))
    {
        //if(strcmp(oldData.name, modifyData.name) != 0){
		if(oldData.memberId != modifyData.memberId){
          //  printf("%d %s %s %d %s\n", oldData.memberId, oldData.name, oldData.email, oldData.eventId, oldData.date);          
			write(g,&oldData,sizeof(oldData));
        }
    }
    write(g,&modifyData,sizeof(modifyData));   
 
    close(members);
    close(g);
   
    remove("members");
    rename("mas", "members");
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
 
void deleteEvent(){
	
	struct Event deleteData;
    struct Event oldData;
	struct Data oldMember;
	
	printf("Rendezveny torlese.");
	
	int event = open("eventfile", O_RDONLY);
    if(event < 0) { perror("Error"); exit(1); }
		
    int g;
    g = open("eventtmp", O_RDWR| O_CREAT | O_APPEND, 0600);
    if(g < 0) { perror("Error"); exit(1); }
   
    printf("\nKerem adja meg a torolni kivant rendezveny ID-jat: ");
    scanf("%d", &deleteData.id);
   
    while(read(event, &oldData, sizeof(oldData)))
    {
        if(deleteData.id != oldData.id){
            write(g,&oldData,sizeof(oldData));
        }
    }
 
    close(event);
    close(g);
   
    int members = open("members", O_RDONLY);
    if(members < 0) { perror("Error"); exit(1); }
	
	int f;
    f = open("memberstmp", O_RDWR| O_CREAT | O_APPEND, 0600);
    if(f < 0) { perror("Error"); exit(1); }
	
	 while(read(members, &oldMember, sizeof(oldMember)))
    {
        if(deleteData.id != oldMember.eventId){		
            write(f,&oldMember,sizeof(oldMember));
        }
    }
   
    close(members);
	close(f);
   
    remove("eventfile");
    rename("eventtmp", "eventfile");
	remove("members");
	rename("memberstmp", "members");
}
 
char* timeStamp()
{
   time_t t = time(NULL);
   struct tm* stt;
   int const dateLen = strlen("xxxx.xx.xx xx:xx:xx") + 1;
   const char* const timeFormat = "%Y.%m.%d %H:%M:%S";
   char* x = (char*)malloc(sizeof(char) * dateLen);
   
   stt = localtime(&t);
   strftime(x, dateLen, timeFormat, stt);
   
   return x;
}

	/*	struct Name *a = &doesntcome;
				 while (a->next != NULL)
				{		
						if(a->next != NULL)
							printf("%s\n", a->n);			
						a = a->next; 	
				}*/