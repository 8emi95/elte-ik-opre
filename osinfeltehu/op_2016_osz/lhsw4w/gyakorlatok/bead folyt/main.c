#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
 
 /*
 TODO List
 - tagokhoz egyéni azonosító
 - fájlok permission szűkítése
 - date egyszerűsítése
 - eventeknél ne lehessen ugyanazt az azonosítót megadni
 */
 
 
struct Data{
    char name[100];
    char email[100];
    int eventId;
    char date[20];
	int memberId;
};
 
struct Event {
    int id;
    char name[30];
};
 
void menu();
void login();
void insertEventFile();
void modify();
void list();
void deleteMember();
void listEvent();
void insertEvent();
void deleteEvent();
char* timeStamp();
 
int main(int argc, char *argv[])
{
	
	if( access( "eventfile", F_OK ) == -1 ) {
		insertEventFile();
		printf("Elsokent adjon meg egy rendezvenyt!\n");
		insertEvent();
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
      printf("2. Rendezveny kiiratasa\n");
      printf("3. Vendeg adatainak modositasa\n");
      printf("4. Vendeg adatainak torlese\n");
      printf("5. Uj rendezveny\n");
      printf("6. Rendezveny kiiratasa\n");
      printf("7. Rendezveny adatainak torlese\n");
      printf("\n");
      printf("Valasztott menupont sorszama: ");
      scanf("%d", &ch);
   } while ( ch < 0 || ch > 7);
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
 
void login()
{
    printf("Uj tag beszurasa.");
   
   //megszámolása az eddigi tagoknak
    int g = open("members", O_RDONLY);
    int count = 0;
    while (read(f, &allData, sizeof(allData)))
    {
        if(newData.eventId == allData.eventId)
            count++;
    }
    close(members);
   
    //elkezdődik a beolvasás
    int members;
    members = open("members", O_WRONLY | O_CREAT | O_APPEND);
   
    struct Data newData;
    struct Data allData;
	struct Event eventData;
       
    printf("\nKerem adja meg a nevet: ");
    scanf("%s", newData.name);
    printf("\nKerem adja meg az e-mail cimet: ");
    scanf("%s", newData.email);
	
	//email formatum ellenorzese
	const char ch1 = '@';
	const char ch2 = '.';
	if( strchr(newData.email, ch1)==NULL || strchr(newData.email, ch2)==NULL ) { perror("Error"); exit(1);}
	
    printf("\nKerem adja meg a rendezveny azonositojat: ");
    scanf("%d", &newData.eventId); 
	
    int event = open("eventfile", O_RDONLY);
    if(event < 0) { perror("Error"); exit(1); }
	
    //ellenőrzése az event létezésének
	int number = 0;
    while (read(event, &eventData, sizeof(eventData)))
    {
			if(newData.eventId == eventData.id)
            number++;
	}
    close(event);
	
	//??
	if(number == 0){ perror("Error"); exit(1); }
	
    char* tmp = timeStamp();
    strcpy(newData.date, tmp);
	
	new
   
    printf("%s %s %d %s\n", newData.name, newData.email, newData.eventId, newData.date);
    write(members,&newData,sizeof(newData));   
    close(members);
 
    int f = open("members", O_RDONLY);
    int c = 0;
    while (read(f, &allData, sizeof(allData)))
    {
        if(newData.eventId == allData.eventId)
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
        printf("%s %s %d %s\n", newData.name, newData.email, newData.eventId, newData.date);
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
   
    printf("\nKerem adja meg a nevet kit akar torolni: ");
    scanf("%s", deleteData.name);
   
    while (read(members, &oldData, sizeof(oldData)))
    {
        if(strcmp(oldData.name, deleteData.name) != 0){
            printf("%s %s %d %s\n", oldData.name, oldData.email, oldData.eventId, oldData.date);
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
   
    printf("\nKerem adja meg a nevet akinel modositana: ");
    scanf("%s", modifyData.name);
    printf("\nUj adatok megadasa:\n");
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
   
    printf("%s %s %d %s\n", modifyData.name, modifyData.email, modifyData.eventId, modifyData.date);
   
    while (read(members, &oldData, sizeof(oldData)))
    {
        if(strcmp(oldData.name, modifyData.name) != 0){
            printf("%s %s %d %s\n", oldData.name, oldData.email, oldData.eventId, oldData.date);
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
        printf("%i %s\n", event.id, event.name);
    }
    close(f);
}
 
void insertEvent()
{
    printf("Uj rendezveny beszurasa.");
   
    int eventfile;
    eventfile = open("eventfile", O_WRONLY | O_APPEND);
    struct Event newEvent;
       
    printf("\nKerem adja meg a rendezveny azonositojat: ");
    scanf("%d", &newEvent.id);
    printf("\nKerem adja meg a rendezveny nevet: ");
    scanf("%s", newEvent.name);
   
    printf("%d %s\n", newEvent.id, newEvent.name);
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