/*
Vári Csaba Géza
F7RMYO
elso beadando
*/

/*
Egy kiállításokat, rendezvényeket szervező társaság a "King of Stands", 
elektronikus látogatói nyilvántartást készített. 
A látogatók a nyilvántartásba jelentkezéskor megadják nevüket, mail címüket, 
a rendezvény azonosító számát! 
Az elektronikus rendszer a jelentkezés nyugtázásaként visszaadja, hogy hányadik vendégként történt meg 
a regisztráció! 

Az alkalmazás adjon lehetőséget vendég jelentkezéshez, a jelentkezettek listázásához, 
egy vendég adatainak módosításához, törléséhez. 
Legyen lehetőségünk egy új rendezvény indításához, egy rendezvény összes adatának törléséhez! 
A jelentkezőknél a korábbi adatok mellett rögzítsük automatikusan a jelentkezési időt(rendszeridő) 
is(ezt nem kell bekérni)! 
*/

/*
guest data:
name, email, event ID, registration time
*/

/*
event data:
ID, name, description?
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> //stat
#include <unistd.h>   //stat
#include <time.h> //ctime
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <errno.h> //perror, errno
#include <string.h>

//display menu
int printMenu()
{
	printf("=====================\n");
	printf("***King of Stands***\n");
	printf("=====================\n");
	
	printf("1: New guest\n");
	printf("2: List guests\n");
	printf("3: Update guest data\n");
	printf("4: Delete guest\n");
	
	printf("-----------------------\n");
	
	printf("5: New event\n");
	printf("6: Delete event data\n");
	
	printf("-----------------------\n");	
	printf("0: Exit\n");
    printf("\n");
	
	printf("Choose an option: \n");
	
	return;
}

//guest struct 
typedef struct {
	char name[80];
	char email[80];
	int eventID;
	char regdate[80];
} guest;

//event struct 
typedef struct {
	int ID;
	char name[80];
	char description[80]; 
} event;

int addGuest()
{
	//todo open file
		
	//todo input
		
	//todo write into file
	
	FILE * f;
	f=fopen("guests.txt","ab");
	if (f==NULL){perror("File opening error\n"); exit(1);}
	
	guest g;
	/*while (!feof(f)){
		fread(&c,sizeof(G),sizeof(G),f); //use fwrite for writing
		printf("%c",c);
	} */
	
	
	printf("Please enter the name of the new guest: \n");
	scanf("%s\n", &g.name);
	getchar();
	
	printf("Please enter the email address: \n");
	scanf(" %s\n", &g.email);
	getchar();
	
	printf("Please enter the ID of the event: \n");
	scanf(" %d\n", &g.eventID);
	//getchar();
	
	//reg date	
	
	time_t curtime;
    time(&curtime); 
	
	strcpy(g.regdate,ctime(&curtime));
	
	fwrite(&g, sizeof(guest), 1, f);
	
	printf("New guest registered. Number: %d\n",countGuests());
	
	
	fclose(f);	
	return;
}

int listGuests()
{
	int i=0; //counting guests
	
	//todo open file
	
	//todo while list
	printf("Listing guests..\n");
	printf("_____________________________\n");
	
	FILE * f;
	f=fopen("guests.txt","rb");
	if (f==NULL){perror("File opening error\n"); exit(1);}
	
	guest g;
	while (!feof(f)){
		i++; //+1 guest
		fread(&g,sizeof(g),sizeof(g),f); //use fwrite for writing
		printf("%g",g);//todo
		printf("%d",i);
	} 
	
	printf("End of list.\n");
	fclose(f);	
	return;
}

int countGuests()
{
	int i=0; //counting guests
	
	//todo open file
	
	//todo while list
	//printf("Listing guests..\n");
	//printf("_____________________________\n");
	
	FILE * f;
	f=fopen("guests.txt","rb");
	if (f==NULL){perror("File opening error\n"); exit(1);}
	
	guest g;
	while (!feof(f)){
		i++; //+1 guest
		fread(&g,sizeof(g),sizeof(g),f); //use fwrite for writing
		//printf("%g",g);//todo
	} 
	
	//printf("End of list.\n");
	fclose(f);	
	return i;
}

int updateGuest()
{
	//todo read guest name or ID
	
	//todo update guest
	
	printf("Please enter the number of the guest you want to update: \n");
	
	printf("Please enter the new name:\n");
	
	printf("Please enter the new email address:\n");
	
	printf("Guest data updated.\n");
	
	return;
}

int delGuest()
{
	// todo read guest name or ID
	
	// todo del guest 
	printf("Please enter the number of the guest you want to delete:\n");
	
	printf("Guest deleted.\n");
	
	return;
}

int addEvent()
{
	//todo read event
	
	//todo write into file
	
	printf("Please enter the name of the new event:\n");
	
	printf("Please enter the date of the new event:\n");
	
	printf("Event added.\n");
	return;
}

int delEvent()
{
	//todo read event name or id
	
	//todo del event
	
	printf("Please enter the ID of the event you wish to delete:\n");
	
	printf("Event data deleted.\n");
	return;
}

int main(int argc,char ** argv) 
{	
 //menu
 
char c;

printMenu();
	
c = getchar();

do {
	
	//menu system		
	switch(c) {
		case '0' :
			exit(0);
		break;
		case '1':
			//adding a guest
			addGuest();
		break;
		case '2':
			//list guests
			listGuests();
		break;
		case '3':
			//change guest data
			updateGuest();
		break;
		case '4':
			//delete guest
			delGuest();
		break;
		case '5':
			//add event
			addEvent();
		break;
		case '6':
			//delete event
			delEvent();
		break;
		default:
			printf("Bad input. Please choose from 0 - 6.\n");			
	}
	
	getchar(); //consume the newline

	printMenu();
	
	c = getchar();
	
} while (!isspace(c));  
 
 return 0;
}