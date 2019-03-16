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

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2

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

// reading from console
static int getLine (char *prmpt, char *buff, size_t sz) {
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prmpt != NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}

int addGuest()
{
	//todo open file
		
	//todo input
		
	//todo write into file
	
	getchar(); //getting rid of buffer from menu
	
	FILE * f;
	f=fopen("guests.txt","ab"); //append binary
	if (f==NULL){perror("File opening error\n"); exit(1);}
	
	guest g;	
	
	int rc;
    char buff[80];

    //rc = getLine ("Enter string> ", buff, sizeof(buff));
    /*if (rc == NO_INPUT) {
        // Extra NL since my system doesn't output that on EOF.
        printf ("\nNo input\n");
        return 1;
    }

    if (rc == TOO_LONG) {
        printf ("Input too long [%s]\n", buff);
        return 1;
    }

    printf ("OK [%s]\n", buff);
	*/
	
	//printf("Please enter the name of the new guest: \n");
	//scanf("%s\n", &g.name);
	rc = getLine ("Please enter the name of the new guest: \n", buff, sizeof(buff));
	//getchar();
	strcpy(g.name, buff);
	//printf("debug: %s\n",g.name);
	
	//printf("Please enter the email address: \n");
	//scanf(" %s\n", &g.email);
	rc = getLine ("Please enter the email address: \n", buff, sizeof(buff));
	//getchar();
	strcpy(g.email, buff);
	//printf("debug: %s\n",g.email);
	
	printf("Please enter the ID of the event: \n");
	scanf("%d", &g.eventID);
	//rc = getLine ("Please enter the ID of the event: \n", buff, sizeof(buff));
	//getchar();
	//g.eventID=buff;
	//printf("debug: %d\n",g.eventID);
	
	//reg date	
	
	time_t curtime;
    time(&curtime); 
	
	strcpy(g.regdate,ctime(&curtime));
	
	printf("Date of registration: %s\n",g.regdate);
	
	fwrite(&g, sizeof(guest), 1, f);
	
	//todo
	printf("New guest registered. Number: %d\n",countGuests());
	
	
	fclose(f);	
	return;
}

int listGuests()
{
	
	//todo open file
	
	//todo while list
	printf("Listing guests..\n");
	printf("_____________________________\n");
	
	FILE * f;
	f=fopen("guests.txt","rb"); //read binary
	//if (f==NULL){perror("File opening error\n"); exit(1);}
	if (f==NULL){perror("Error: Guest file not found. Maybe there are no guests.\n"); return;}
	
	guest g;
	
	/*
	while (!feof(f)){
		//fread(&g,sizeof(g),sizeof(g),f); //use fwrite for writing
		fread(&g,sizeof(g),1,f); //use fwrite for writing
		//todo		
		printf("%s | ",g.name);
		printf("%s | ",g.email);
		printf("%d | ",g.eventID);
		printf("%s \n",g.regdate);
	} 
	*/
	
	fread(&g,sizeof(g),1,f); //use fwrite for writing
	
	while (!feof(f)){
		//fread(&g,sizeof(g),sizeof(g),f); //use fwrite for writing
		//fread(&g,sizeof(g),1,f); //use fwrite for writing
		//todo		
		printf("%s | ",g.name);
		printf("%s | ",g.email);
		printf("%d | ",g.eventID);
		printf("%s",g.regdate);
		
		fread(&g,sizeof(g),1,f); //use fwrite for writing
	} 
	
	printf("\n___End of list.___\n\n");
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
	fread(&g,sizeof(g),1,f); //use fwrite for writing
	i++;
	while (!feof(f)){
		i++;
		//fread(&g,sizeof(g),sizeof(g),f); //use fwrite for writing
		//fread(&g,sizeof(g),1,f); //use fwrite for writing
		//todo		
		/*printf("%s | ",g.name);
		printf("%s | ",g.email);
		printf("%d | ",g.eventID);
		printf("%s \n",g.regdate);
		*/
		fread(&g,sizeof(g),1,f); //use fwrite for writing		
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