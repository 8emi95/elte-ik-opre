#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <time.h>
#include <string.h>

struct guest{
	char name[30];
	char email[50];
	time_t date;
	int noOfEvent;
};

struct event{
	int noOfEvent;
	char eventName[30];
	int guestCounter;
	struct guest guests[20];
};
int eventCounter;
struct event eventPointer[10];

int writeEventsToFile()
{	
		FILE *fileToWrite;			
		fileToWrite=fopen("events.txt","wb");
		if (!fileToWrite){
			printf("nem lehet fajlt letrehozni");
			return 1;
		}else{
		printf("fajl letrehozva events.txt! \n");
		}
		
		fwrite(&eventCounter, sizeof(int), sizeof(int), fileToWrite);	
		fwrite(eventPointer,sizeof(struct event),sizeof(struct event)*eventCounter,fileToWrite);

		fclose(fileToWrite);
		return 0;
}

int readEventsFromFile()
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


		fread(&eventCounter, sizeof(int), sizeof(int), fileToRead);
		fread(eventPointer,sizeof(struct event),sizeof(struct event)*eventCounter,fileToRead);

		fclose(fileToRead);
		return 0;	
}

void deleteGuest(){
	char buffer[256];   
	printf("\n Kérem a torolni kivant vendeg nevet :  \n");
	read(1,buffer,256);
	char nametodelete[256];
	strcpy(nametodelete,buffer);
	
	printf("\n Kerem a torolni kivant vendeg email cimet : \n");
	buffer[0]='\0';
	read(1,buffer,256);
	char emailtodelete[256];
	strcpy(emailtodelete,buffer);
	
	printf("%s %s",nametodelete,emailtodelete);
	
	int i;
	for(i = 0;i<eventCounter;i++){
		int j;
		for(j = 0;j<eventPointer[i].guestCounter;j++){
			if(strcmp(eventPointer[i].guests[j].name,nametodelete) == 0 && strcmp(eventPointer[i].guests[j].email,emailtodelete) == 0){
				int k;
				for(k = j;k<eventPointer[i].guestCounter-1;k++){
					eventPointer[i].guests[k] = eventPointer[i].guests[k+1];
				}
				printf("Torolve!");
				eventPointer[i].guestCounter--;
			} 
		}
	}	
	
}

void showEvents(){	
	int i;
	for(i = 0;i < eventCounter;i++){
		printf("%d %s \n",eventPointer[i].noOfEvent,eventPointer[i].eventName);		
	}
}

void programExit(){
	writeEventsToFile();
	exit(0);
}

void registerNewGuest()
{
	char buffer[256];   
	printf("\n Kerem az esemeny sorszamat \n");
	read(1,buffer,256);
	int i;
	int number = -1;;
	for(i = 0;i<eventCounter;++i){
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
	buffer[0]='\0';
	read(1,buffer,256);
	strcpy(eventPointer[number].guests[eventPointer[number].guestCounter].name,buffer);
	
	printf("\n Kerem a vendeg email cimet \n");
	buffer[0]='\0';
	read(1,buffer,256);
	strcpy(eventPointer[number].guests[eventPointer[number].guestCounter].email,buffer);
	eventPointer[number].guestCounter++;
	
	eventPointer[number].guests[eventPointer[number].guestCounter].date = time(0);	
	
	printf("A vendeg sorszama: %d",eventPointer[number].guestCounter);
}

void modifyGuest()
{
	printf("\n Add meg a vendeg nevet, amit modositani szeretnel:\n");
	char buffer[256];    
	read(1,buffer,256);
	char nametofind[256];
	strcpy(nametofind,buffer);
	
	printf("\n Add meg a vendeg email cimet,amit modositani szeretnel \n");
	buffer[0]='\0';
	read(1,buffer,256);
	char email[256];
	strcpy(email,buffer);	
	
	printf("\n Add meg a vendeg nevet, amire modositani szeretnel:\n");
	buffer[0]='\0';
	read(1,buffer,256);
	char nametoeditto[256];
	strcpy(nametoeditto,buffer);
	
	printf("\n Add meg a vendeg email cimet,amire modositani szeretnel \n");
	buffer[0]='\0';
	read(1,buffer,256);
	char emailtoeditto[256];
	strcpy(emailtoeditto,buffer);	
	
	int i;
	for(i = 0;i<eventCounter;i++){
		int j;
		for(j = 0;j<eventPointer[i].guestCounter;j++){
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
	for(i = 0;i<eventCounter;i++){
		if(eventPointer[i].noOfEvent == atoi(buffer)){
			int j;
			for(j = 0;j<eventPointer[i].guestCounter;j++){
				printf("Vendeg: %sEmail cime: %s\n",eventPointer[i].guests[j].name,eventPointer[i].guests[j].email);
			}
		}
	}
}

void createNewEvent(){
	printf("\n Add meg a rendezveny nevet:\n");
	char buffer[256];    
	read(1,buffer,256);
	strcpy(eventPointer[eventCounter].eventName,buffer);
	printf("\n Add meg az esemeny sorszamat \n");
	buffer[0]='\0';
	read(1,buffer,256);
	eventPointer[eventCounter].noOfEvent = atoi(buffer);	
	eventPointer[eventCounter++].guestCounter = 0;	
	
}
void deleteEvent(){
	printf("\n Add meg a rendezveny sorszamat, amelyiket torolni szeretned:\n");
	char buffer[256];   	
	read(1,buffer,256);
	int i;
	int number = -1;
	for(i = 0;i<eventCounter;i++){
		if(eventPointer[i].noOfEvent == atoi(buffer)){
			number = i;
			int j;
			for(j = i;j<eventCounter-1;j++){
				eventPointer[j] = eventPointer[j+1];
			}
			eventCounter--;
		}
	}
	if(number == -1){
		printf("\n Nem talalhato a megadott sorszamu esemeny");
		exit(1);
	}
}

void showMenuMessage(){
	   printf("\n 0-Exit \n 1-Vendeg jelentkezes \n 2-Jelentkezettek listazasa \n 3-Vendeg adatainak modositasa\n 4-Vendeg torlese a listarol \n 5-Uj rendezveny inditasa \n 6-Addott rendezveny osszes adatanak torlese \n");
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
                                case 7: showEvents();typedChar = -1; showMenuMessage();break;
                                default: printf("Nem ertem, hogy mit akarsz "); typedChar = -1;
                        }

        }while(typedChar == -1);
}


void init(){
	if(readEventsFromFile() == 1){
		eventCounter = 0;		
	}
}
int main(){
	init();
        showmenu();

        return 0;
}
