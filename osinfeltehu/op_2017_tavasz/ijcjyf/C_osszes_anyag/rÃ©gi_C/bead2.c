#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <time.h>
#include <string.h>
#include  <sys/types.h>
#include <sys/wait.h>


struct guest{
	char name[30];
	char email[50];
	time_t date;
	int noOfEvent;
};

struct event{
	int noOfEvent;
	char eventName[30];
	int guestCounterer;
	struct guest guests[20];
};
int eventCounterer;
struct event eventPointer[10];

void clearBuffer(char *array) {
    int i = 0;
    for (i = 0; i < 256; i++) {
        array[i] = (char) 0;
    }
}

int writeToFile()
{	
		FILE *fileToWrite;			
		fileToWrite=fopen("events.txt","wb");
		if (!fileToWrite){
			printf("nem lehet fajlt letrehozni");
			return 1;
		}else{
		printf("fajl letrehozva events.txt! \n");
		}
		
		fwrite(&eventCounterer, sizeof(int), sizeof(int), fileToWrite);	
		fwrite(eventPointer,sizeof(struct event),sizeof(struct event)*eventCounterer,fileToWrite);

		fclose(fileToWrite);
		return 0;
}

int readFromFile()
{	
		FILE *fileToRead;		
		fileToRead=fopen("proba.txt","rb");
		if (!fileToRead)
		{
			printf("Nem sikerült megnyitni a fajlt!");
			return 1;
		}else{
		printf("fajl beolvasva proba.txt! \n");
		}


		fread(&eventCounterer, sizeof(int), sizeof(int), fileToRead);
		fread(eventPointer,sizeof(struct event),sizeof(struct event)*eventCounterer,fileToRead);

		fclose(fileToRead);
		return 0;	
}

void deleteGuest(){
	printf("\n Kérem a torolni kivant vendeg email cimet :  \n");
	char buffer[256];   
	read(1,buffer,256);
	char emailtodelete[256];
	strcpy(emailtodelete,buffer);
	
	
	
	int i;
	for(i = 0;i<eventCounterer;i++){
		int j;
		for(j = 0;j<eventPointer[i].guestCounterer;j++){
			if(strcmp(eventPointer[i].guests[j].email,emailtodelete) == 0){
				int k;
				for(k = j;k<eventPointer[i].guestCounterer-1;k++){
					eventPointer[i].guests[k] = eventPointer[i].guests[k+1];
				}
				printf("Torolve!");
				eventPointer[i].guestCounterer--;
			} 
		}
	}	
	
}

void showEvents(){	
	int i;
	for(i = 0;i < eventCounterer;i++){
		printf("%d %s \n",eventPointer[i].noOfEvent,eventPointer[i].eventName);		
	}
}

void programExit(){
	writeToFile();
	exit(0);
}

void registerNewGuest()
{
	char buffer[256];   
	printf("\n Kerem az esemeny sorszamat \n");
	read(1,buffer,256);
	int i;
	int number = -1;
	for(i = 0;i<eventCounterer;++i){
		if(eventPointer[i].noOfEvent == atoi(buffer)){
			number = i;
			break;			
		}
	}
	if(number == -1){
		printf("Ilyen sorszamu esemenyt nem talaltunk \n");
		exit(1);
	}
	printf("\n Kerem a vendeg nevet \n");
	clearBuffer(buffer);
	read(1,buffer,256);
	strcpy(eventPointer[number].guests[eventPointer[number].guestCounterer].name,buffer);
	
	printf("\n Kerem a vendeg email cimet \n");
	clearBuffer(buffer);
	read(1,buffer,256);
	strcpy(eventPointer[number].guests[eventPointer[number].guestCounterer].email,buffer);
	eventPointer[number].guestCounterer++;
	
	eventPointer[number].guests[eventPointer[number].guestCounterer].date = time(0);	
	
	printf("A vendeg sorszama: %d",eventPointer[number].guestCounterer);
}

void modifyGuest()
{
	printf("\n Add meg a vendeg nevet, amit modositani szeretnel:\n");
	char buffer[256];    
	read(1,buffer,256);
	char nametofind[256];
	strcpy(nametofind,buffer);
	
	printf("\n Add meg a vendeg email cimet,amit modositani szeretnel \n");
	clearBuffer(buffer);
	read(1,buffer,256);
	char email[256];
	strcpy(email,buffer);	
	
	printf("\n Add meg a vendeg nevet, amire modositani szeretnel:\n");
	clearBuffer(buffer);
	read(1,buffer,256);
	char nametoeditto[256];
	strcpy(nametoeditto,buffer);
	
	printf("\n Add meg a vendeg email cimet,amire modositani szeretnel \n");
	clearBuffer(buffer);
	read(1,buffer,256);
	char emailtoeditto[256];
	strcpy(emailtoeditto,buffer);	
	
	int i;
	for(i = 0;i<eventCounterer;i++){
		int j;
		for(j = 0;j<eventPointer[i].guestCounterer;j++){
			if(strcmp(eventPointer[i].guests[j].name,nametofind) == 0 && strcmp(eventPointer[i].guests[j].email,email) == 0){
				strcpy(eventPointer[i].guests[j].name,nametoeditto);
				strcpy(eventPointer[i].guests[j].email,emailtoeditto);
			}
		}
	}
}

void showAddedGuests()
{
	char buffer[256];   
	printf("\n Kerem az esemeny sorszamat \n");
	read(1,buffer,256);
	int i;
	for(i = 0;i<eventCounterer;i++){
		if(eventPointer[i].noOfEvent == atoi(buffer)){
			int j;
			for(j = 0;j<eventPointer[i].guestCounterer;j++){
				printf("Vendeg: %sEmail cime: %s\n",eventPointer[i].guests[j].name,eventPointer[i].guests[j].email);
			}
		}
	}
}

void createNewEvent(){
	printf("\n Add meg a rendezveny nevet:\n");
	char buffer[256];    
	read(1,buffer,256);
	strcpy(eventPointer[eventCounterer].eventName,buffer);
	printf("\n Add meg az esemeny sorszamat \n");
	clearBuffer(buffer);
	read(1,buffer,256);
	eventPointer[eventCounterer].noOfEvent = atoi(buffer);	
	eventPointer[eventCounterer++].guestCounterer = 0;	
	
}
void deleteEvent(){
	printf("\n Add meg a rendezveny sorszamat, amelyiket torolni szeretned:\n");
	char buffer[256];   	
	read(1,buffer,256);
	int i;
	int number = -1;
	for(i = 0;i<eventCounterer;i++){
		if(eventPointer[i].noOfEvent == atoi(buffer)){
			number = i;
			int j;
			for(j = i;j<eventCounterer-1;j++){
				eventPointer[j] = eventPointer[j+1];
			}
			eventCounterer--;
		}
	}
	if(number == -1){
		printf("\n Nem talalhato a megadott sorszamu esemeny");
		exit(1);
	}
}

void showMenuMessage(){
	   printf("\n 0-Exit \n 1-Vendeg jelentkezes \n 2-Jelentkezettek listazasa \n 3-Vendeg adatainak modositasa\n 4-Vendeg torlese a listarol \n 5-Uj rendezveny inditasa \n 6-Addott rendezveny osszes adatanak torlese \n 7-Rendezveny lebonyolitasa \n");
}

void handler(int signumber){
  printf("Varom az adatokat\n");
}

void executeEvent() {
    int pipefd[2];
    pid_t pid;
    char m[256];
    struct guest guests[20];
    struct guest didntshowup[20];
    int didntshowupcount;
    signal(SIGTERM,handler);
    srand(time(NULL));
    printf("Add meg az esemeny sorszamat, amit el szeretnel inditani: ");    
    char buffer[256];
    read(1, buffer, 256);
    int i = 0;
    for (i = 0; i < eventCounterer; i++) {
        if (eventPointer[i].noOfEvent == atoi(buffer)) {
            clearBuffer(buffer);
            printf("\n Add meg az esemeny helyszinet:");
            fflush( stdout );
            read(1, buffer, 256);
            if (pipe(pipefd) == -1) {
                perror("Hiba a pipe nyitaskor!");
                exit(EXIT_FAILURE);
            }
            pid = fork();
            if (pid == -1) {
                perror("Fork hiba");
                exit(EXIT_FAILURE);
            }

            if (pid == 0) { // child process                
                read(pipefd[0], m, sizeof (m)); 
                sleep(5);
                printf("Itt vagyok: %s", m);
                kill(getppid(),SIGTERM); 
                printf("\n");
                sleep(1);
                read(pipefd[0],guests, eventPointer[i].guestCounterer*sizeof(struct guest));
                int j;
                for(j = 0;j<eventPointer[i].guestCounterer;j++){
                    printf("vendeg: %s",guests[j].name);
                    int generated = (rand() % 100 + 1);                    
                    if(generated>90){
                        didntshowup[didntshowupcount] = guests[j];
                        didntshowupcount++;
                    }
                }
                write(pipefd[1],&didntshowupcount,sizeof(didntshowupcount));
                write(pipefd[1],&didntshowup,didntshowupcount*sizeof(struct guest));
                kill(getppid(),SIGTERM); 
                
                close(pipefd[0]);
            } else {
             
                write(pipefd[1], buffer, 256);
                
                fflush(NULL);
                pause();
                
                write(pipefd[1], &eventPointer[i].guests, eventPointer[i].guestCounterer*sizeof(struct guest));
                fflush(NULL);
                pause();
                read(pipefd[0],didntshowup,sizeof(didntshowupcount));
                read(pipefd[0],didntshowup,didntshowupcount*sizeof(struct guest));
                int j;        
                printf("\n Akik nem erkeztek meg:\n");                
                for(j = 0;j<didntshowupcount;j++){
                    printf("%s",didntshowup[j].name);
                }                
                showMenuMessage();
            }            
        }
    }
}

void showmenu(){
        int typedChar = -1;
		showMenuMessage();
        do{
                        scanf("%d",&typedChar);
                        switch(typedChar){
                                case 0: programExit();break;
                                case 1: registerNewGuest();typedChar = -1; showMenuMessage();break;
                                case 2: showAddedGuests();typedChar = -1; showMenuMessage();break;
                                case 3: modifyGuest();typedChar = -1; showMenuMessage();break;
                                case 4: deleteGuest();typedChar = -1; showMenuMessage();break;
                                case 5: createNewEvent(); typedChar = -1; showMenuMessage();break;
                                case 6: deleteEvent();typedChar = -1; showMenuMessage();break;                                
                                case 7: executeEvent();typedChar = -1;break;
                                default: printf("Nem ertem, hogy mit akarsz "); typedChar = -1;
                        }

        }while(typedChar == -1);
}


void init(){
	if(readFromFile() == 1){
		eventCounterer = 0;		
	}
}
int main(){
	setbuf(stdout, NULL);
	init();
    showmenu();

    return 0;
}
