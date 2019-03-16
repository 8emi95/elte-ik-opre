#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>  // for errno, the number of last error
#include <time.h>
#include <signal.h>
#include <errno.h>

#define EVENTLIMIT 100

struct Guest{
	char name[100];
	char email[100];
	time_t tm;
};

struct Event{
	int GuestNumber;
	struct Guest* Guests;
};

int EventNumber = 0;
struct Event Events[EVENTLIMIT];

void NewEvent();
void DelEvent();
void AddGuest();
void ListGuests();
void EditGuest();
void DelGuest();
void LodaFile();
void SaveFile();
void LeadEvent();

int main()
{
LodaFile();
	char menu[1];
do {
	printf("Aktualis esemenyek azonositok: ");
	int i=0;
	for(i;i < EventNumber;i++)
		printf("%d ",i);
	printf("\n");
    printf("Mit szeretne csinalni?\n");
	printf("1, Vendeg jelentkezes.\n");
	printf("2, Jelentkezett vendegek listazasa.\n");
	printf("3, Vendeg adatainak modositasa.\n");
	printf("4, Vendeg torlese.\n");
	printf("5, Uj rendezveny inditasa.\n");
	printf("6, Rendezveny adatainak torlese.\n");
	printf("7, Rendezveny lebonyolitasa.\n");
	printf("8, Kilepes!\n");
	scanf("%s",&menu);
	switch(*menu){
		case '1' : 
			AddGuest();
			break;
		case '2' : 
			ListGuests();
			break;
		case '3' : 
			EditGuest();
			break;
		case '4' : 
			DelGuest();
			break;
		case '5' : 
			NewEvent();
			break;
		case '6' :
			DelEvent();
			break;
		case '7' :
			LeadEvent();
			break;
	}
SaveFile();
}while (*menu!='8');
    return 1;
}


void LeadEvent(){
 int fd1,fd2;
 unlink("rendid");
 unlink("info");
 int fid1=mkfifo("rendid", S_IRUSR|S_IWUSR ); 
 int fid2=mkfifo("info", S_IRUSR|S_IWUSR ); 
  if (fid1==-1){
    printf("Error number: %i",errno); 
    exit(EXIT_FAILURE);
   }
  if (fid2==-1){
    printf("Error number: %i",errno);
    exit(EXIT_FAILURE);
   }
 int Eventid = GetEventID();
  if(Eventid < 0 || Eventid >= EventNumber){
    printf("Ajjaj! Rossz azonositot adtal meg!\n");
   } else {
    pid_t pid = fork();

    if(pid>0){ //parent
      char id[10];
      char message[10];
      sprintf(id,"%d",Eventid);
      fd1=open("rendid",O_WRONLY);
      write(fd1,id,sizeof(id));
      close(fd1);

      fd1=open("rendid",O_RDONLY);		
      read(fd1,message,sizeof(message));
      close(fd1);
      int arrived=atoi(message);
      if(arrived == -1){
        printf("\"Rendben van fiam kuldom az adatokat!\"\n");
	char *str;
	str =(char*)malloc(Events[Eventid].GuestNumber*101*sizeof(char));
	int i;
	int k=0;
	for(i=0;i<Events[Eventid].GuestNumber;i++){
	  int j=0;
	  char tmp[sizeof(Events[Eventid].Guests[i].name)];
	  sprintf(tmp,"%s!",Events[Eventid].Guests[i].name);
	  strcat(str,tmp);
	}
	printf("%s",str);
	fd2=open("info",O_WRONLY);
        write(fd2,str,sizeof(str));
        close(fd2);
       } else {
        printf("Nem ertem mi tortent!\n");
       }
      unlink("rendid");
      unlink("info");
     } else { // child
      char message[10];
      fd1=open("rendid",O_RDONLY);		
      read(fd1,message,sizeof(message));
      printf("A partner elindul a rendezveny helyszinere!\n");
      sleep(2);
      int id=atoi(message);

      printf("A partner megerkezett a %d azonositoju rendezvenyre. \n",id);
      printf("\"Hallo Fonok! Megerkeztem! Varom a tovabbi utasitasokat! Kik a resztvevok?\"\n");

      fd1=open("rendid",O_WRONLY);
      sprintf(message,"-1");
      write(fd1,message,sizeof(message));
      close(fd1);

      char guests[Events[id].GuestNumber*101];
      printf("%d",sizeof(guests));
      fd2=open("info",O_RDONLY);
      //fgets(guests, sizeof(guests), stdin);
      //scanf(fd2,"%s",guests);	
      read(fd2,guests,sizeof(guests));
      close(fd2);
      int i;
      int k=0;
      char* name;
      name =(char*)malloc(101*sizeof(char));
      for(i=0;i<sizeof(guests);i++){
	if(guests[i] == '!'){
          printf("%s\n",name);
	  memset(name, 0, sizeof(name));
	  k=0;
	 } else {
	  name[k++]=guests[i];
         }
       }
      printf("%s\n",guests);
      printf("\n");
      exit(2);
     }	
   }
}

void AddGuest(){
	int id=GetEventID();
	if (id >=EventNumber || id < 0 ){
		printf("Hibas esemenyazonosito\n");
		return;
	}	
	char name[100];
    char email[100];

    printf("Nev: \n");
    fgets(name, 100, stdin); 
    scanf("%[^\n]*s", name);

    printf("Email: \n");
    fgets(email, 100, stdin); 
    scanf("%[^\n]*s", email);

    int GuestNumber = Events[id].GuestNumber++;
    strcpy(Events[id].Guests[GuestNumber].name, name);
    strcpy(Events[id].Guests[GuestNumber].email, email);
	time(&Events[id].Guests[GuestNumber].tm);
	printf("Vendeg hozzaadva, azonositoja: %i\n", GuestNumber);
}

void ListGuests() {
int id=GetEventID();
if (id >=EventNumber || id < 0 ){
		printf("Hibas esemenyazonosito\n");
		return;
	}
	
	int i;
	for(i=0;i<Events[id].GuestNumber;i++){
		printf("Vendeg azonositoja: %d \n",i);
		printf("Nev: %s\n",Events[id].Guests[i].name);
		printf("Email: %s\n",Events[id].Guests[i].email);
		printf("Jelentkezesi datum: %s\n",ctime(&Events[id].Guests[i].tm));
	}
}

void EditGuest(){
int id=GetEventID();
if (id >=EventNumber || id < 0 ){
		printf("Hibas esemenyazonosito\n");
		return;
	}
printf("Aktualis vendeg azonositok: ");
	int i=0;
	for(i;i < Events[id].GuestNumber;i++)
		printf("%d ",i);
	printf("\n");
int GuestID;
printf("Adja meg a vendeg azonositojat:\n");
scanf("%d",&GuestID);
if (Events[id].GuestNumber<=GuestID || GuestID < 0 ){
		printf("Hibas vendegazonosito\n");
		return;
	}
printf("Jelenlegi adatok:\n");
		printf("Nev: %s\n",Events[id].Guests[GuestID].name);
		printf("Email: %s\n",Events[id].Guests[GuestID].email);

printf("Adja meg az uj adatokat:\n");

	char name[100];
    char email[100];

    printf("Nev: \n");
    fgets(name, 100, stdin); 
    scanf("%[^\n]*s", name);

    printf("Email: \n");
    fgets(email, 100, stdin); 
    scanf("%[^\n]*s", email);
strcpy(Events[id].Guests[GuestID].name, name);
strcpy(Events[id].Guests[GuestID].email, email);
}

void DelGuest(){
int id=GetEventID();
if (id >=EventNumber || id < 0 ){
		printf("Hibas esemenyazonosito\n");
		return;
	}
printf("Aktualis vendeg azonositok: ");
	int i=0;
	for(i;i < Events[id].GuestNumber;i++)
		printf("%d ",i);
	printf("\n");
int GuestID;
printf("Adja meg a vendeg azonositojat:\n");
scanf("%d",&GuestID);
if (Events[id].GuestNumber<=GuestID || GuestID < 0 ){
		printf("Hibas vendegazonosito\n");
		return;
	}
if(Events[id].GuestNumber==GuestID){
		Events[id].GuestNumber--;
	}else{
		Events[id].GuestNumber--;
		Events[id].Guests[GuestID]=Events[id].Guests[Events[id].GuestNumber];
	}
}

int GetEventID(){
printf("Adja meg az esemeny azonositojat:\n");
		int id;
		scanf("%i",&id);
	return id;
}

void NewEvent() {
if (EventNumber >= EVENTLIMIT){
	printf("Tobb esemeny nem adhato hozza!\n");
	return;
}
	
	Events[EventNumber].Guests=(struct Guest*)malloc(50*sizeof(struct Guest));
	Events[EventNumber].GuestNumber=0;
	printf("Esemeny hozzaadva, azonositoja: %i\n", EventNumber);
	EventNumber++;
}

void DelEvent(){
	int id=GetEventID();
	if (id >=EventNumber || id < 0 ){
		printf("Hibas esemenyazonosito\n");
		return;
	}
	Events[id].GuestNumber=0;
	Events[id].Guests=realloc(Events[id].Guests,(50*sizeof(struct Guest)));
	
}

void SaveFile(){
	FILE* fp;
    fp = fopen("out.txt", "w");

    if (fp == NULL){
        printf("Hiba a mentesnel!!!");
        return;
    }
	
	fwrite( &EventNumber, sizeof(int), 1, fp);

    int i;
    for (i = 0; i < EventNumber; i++) {
        fwrite( &Events[i].GuestNumber, sizeof(int), 1, fp);
        fwrite( Events[i].Guests, sizeof(*Events[i].Guests), Events[i].GuestNumber, fp);
    }

    fclose(fp);
}

void LodaFile(){
	FILE *fp;
fp=fopen("out.txt", "r");
	
	if (fp == NULL){
        printf("Betoltes sikertelen!!");
        return;
    }
	
fread(&EventNumber, sizeof(int),1, fp);
    int i;
    for (i = 0; i < EventNumber; i++) {
        fread( &Events[i].GuestNumber, sizeof(int), 1, fp);
        Events[i].Guests = (struct Guest*)malloc(50*sizeof(struct Guest));
        fread( Events[i].Guests, sizeof(*Events[i].Guests), Events[i].GuestNumber, fp);
    }
};
