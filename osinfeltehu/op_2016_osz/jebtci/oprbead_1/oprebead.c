#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <time.h>
#include <string.h>

struct guest{
	char name[30];
	char email[50];
	time_t date;
	int eventNumber;
};

struct event{
	int eventNumber;
	char eventName[30];
	int guestCount;
	struct guest guests[20];
};
int eventCount;
struct event eventPointer[10];

int writeEventsToFile()
{	
		FILE *fileToWrite;			
		fileToWrite=fopen("events.bin","wb");
		if (!fileToWrite){
			printf("nem lehet fajlt letrehozni");
			return 1;
		}
		
		fwrite(&eventCount, sizeof(int), sizeof(int), fileToWrite);	
		fwrite(eventPointer,sizeof(struct event),sizeof(struct event)*eventCount,fileToWrite);

		fclose(fileToWrite);
		return 0;
}

int readEventsFromFile()
{	
		FILE *fileToRead;		
		fileToRead=fopen("events.bin","rb");
		if (!fileToRead)
		{
			printf("Nem sikerült megnyitni a fajlt!");
			return 1;
		}

		fread(&eventCount, sizeof(int), sizeof(int), fileToRead);
		fread(eventPointer,sizeof(struct event),sizeof(struct event)*eventCount,fileToRead);

		fclose(fileToRead);
		return 0;	
}
void showEvents(){	
	int i;
	for(i = 0;i < eventCount;i++){
		printf("%d %s \n",eventPointer[i].eventNumber,eventPointer[i].eventName);		
	}
}
void handleExit(){
	writeEventsToFile();
	exit(0);
}
void deleteGuest(){
	
	printf("\n Add meg a torolni kivant vendeg email cimet \n");
	char buffer[256];
	read(1,buffer,256);
	char emailtodelete[256];
	strcpy(emailtodelete,buffer);	
	
	int i;
	for(i = 0;i<eventCount;i++){
		int j;
		for(j = 0;j<eventPointer[i].guestCount;j++){
			if(strcmp(eventPointer[i].guests[j].email,emailtodelete) == 0){
				int k;
				for(k = j;k<eventPointer[i].guestCount-1;k++){
					eventPointer[i].guests[k] = eventPointer[i].guests[k+1];
				}
				printf("toroltunk");
				eventPointer[i].guestCount--;
			} 
		}
	}	
	
}
void registerNewGuest()
{
	char buffer[256];   
	printf("\n Add meg az esemeny sorszamat \n");
	read(1,buffer,256);
	int i;
	int number = -1;;
	for(i = 0;i<eventCount;++i){
		if(eventPointer[i].eventNumber == atoi(buffer)){
			number = i;
			break;			
		}
	}
	if(number == -1){
		printf("Nem talalhato ilyen sorszamu esemeny\n");
		exit(1);
	}
	printf("\n Add meg a vendeg nevet \n");
	buffer[0]='\0';
	read(1,buffer,256);
	strcpy(eventPointer[number].guests[eventPointer[number].guestCount].name,buffer);
	
	printf("\n Add meg a vendeg email cimet \n");
	buffer[0]='\0';
	read(1,buffer,256);
	strcpy(eventPointer[number].guests[eventPointer[number].guestCount].email,buffer);
	eventPointer[number].guestCount++;
	
	eventPointer[number].guests[eventPointer[number].guestCount].date = time(0);	
	
	printf("A vendeg sorszama: %d",eventPointer[number].guestCount);
}
void showRegisteredGuests()
{
	char buffer[256];   
	printf("\n Add meg az esemeny sorszamat \n");
	read(1,buffer,256);
	int i;
	for(i = 0;i<eventCount;i++){
		if(eventPointer[i].eventNumber == atoi(buffer)){
			int j;
			for(j = 0;j<eventPointer[i].guestCount;j++){
				printf("A vendeg neve: %semail cime: %s\n",eventPointer[i].guests[j].name,eventPointer[i].guests[j].email);
			}
		}
	}
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
	for(i = 0;i<eventCount;i++){
		int j;
		for(j = 0;j<eventPointer[i].guestCount;j++){
			if(strcmp(eventPointer[i].guests[j].name,nametofind) == 0 && strcmp(eventPointer[i].guests[j].email,email) == 0){
				strcpy(eventPointer[i].guests[j].name,nametoeditto);
				strcpy(eventPointer[i].guests[j].email,emailtoeditto);
			}
		}
	}
}
void createNewEvent(){
	printf("\n Add meg a rendezveny nevet:\n");
	char buffer[256];    
	read(1,buffer,256);
	strcpy(eventPointer[eventCount].eventName,buffer);
	printf("\n Add meg az esemeny sorszamat \n");
	buffer[0]='\0';
	read(1,buffer,256);
	eventPointer[eventCount].eventNumber = atoi(buffer);	
	eventPointer[eventCount++].guestCount = 0;	
	//showEvents();	
	
}
void deleteEvent(){
	printf("\n Add meg a rendezveny sorszamat, amelyiket torolni szeretned:\n");
	char buffer[256];   	
	read(1,buffer,256);
	int i;
	int number = -1;
	for(i = 0;i<eventCount;i++){
		if(eventPointer[i].eventNumber == atoi(buffer)){
			number = i;
			int j;
			for(j = i;j<eventCount-1;j++){
				eventPointer[j] = eventPointer[j+1];
			}
			eventCount--;
		}
	}
	if(number == -1){
		printf("\n Nem talalhato a megadott sorszamu esemeny");
		exit(1);
	}
}

void showMenuMessage(){
	   printf("\n 1-Vendeg jelentkezese \n 2-Jelentkezettek listaja \n 3-Vendeg adatainak modositasa\n 4-Vendeg torlese \n 5-Uj rendezveny letrehozasa \n 6-Rendezveny osszes adatanak torlese \n");
}
void showmenu(){
        int typedChar = -1;
		showMenuMessage();
        do{
                        scanf("%d",&typedChar);
                        switch(typedChar){
                                case 0: handleExit();break;
                                case 1: registerNewGuest();typedChar = -1; showMenuMessage();break;
                                case 2: showRegisteredGuests();typedChar = -1; showMenuMessage();break;
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
		eventCount = 0;		
	}
}
int main(){
	init();
        showmenu();

        return 0;
}
