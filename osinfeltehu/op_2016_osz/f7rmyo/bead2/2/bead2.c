/*
F7RMYO
masodik beadando
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
A "King of Stands" rendezvény szervező társaság, amint a rendezvény ideje eljön, megbízza egy 
partnerét(gyermekét) az esemény lebonyolítására. A gyermekfolyamat indulása után csővezetéken megkapja, 
hogy hol kerül a rendezvény lebonyolításra! A helyszínre utazik, majd visszajelzi (jelzést küld vissza) 
a központnak, hogy a megadott helyszinen van, várja a résztvevőket,kész a rendezvény lebonyolítására. 
Ezután a rendezvényszervező csővezetéken elküldi a helyszínen tartózkodó partnernek, hogy kik a 
résztvevők. Ezt a partner képernyőre írja, majd várja a vendégeket és levezényli a rendezvényt! 
A rendezvény végén jelentést küld vissza, amiben tájékoztatja a főszervezőt, hogy mennyire sikeres volt 
a rendezvény (véletlenszám), illetve a jelentkezettek közül végül is ki nem érkezett meg! 
(Minden résztvevő 10% valószínűséggel nem érkezik meg!) Miután egy esemény lebonyolításra került, 
"King of Stands" újra megvizsgálja, melyik rendezvény a következő! 
*/

/*
guest data:
name, email, event ID, registration time
*/

/*
event data:
ID, name, location, date?
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
#include <sys/sendfile.h>
#include <signal.h>


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
	printf("6: List events\n");
	printf("7: Delete event data\n");
	
	printf("-----------------------\n");
	printf("8: Start event management\n");
	printf("-----------------------\n");
	
	printf("-----------------------\n");		
	printf("9: Load sample files for testing\n");
	printf("-----------------------\n");		
	printf("0: Exit\n");
    printf("\n");
	
	printf("Choose an option: \n");
	
	return;
}

//guest struct 
typedef struct {
	//int guestID;
	char name[80];
	char email[80];
	int eventID;
	char regdate[80];
} guest;

//event struct 
typedef struct {
	int ID;
	char name[80];
	char location[80]; 
	char eventdate[80];
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

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int addGuest()
{	
	getchar(); //getting rid of buffer from menu
	
	FILE * f;
	f=fopen("guests.dat","ab"); //append binary
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
	
	//g.guestID=countGuests()+1; //recording guest ID automatically
	
	printf("Date of registration: %s\n",g.regdate);
	
	fwrite(&g, sizeof(guest), 1, f); //record into file
	
	
	printf("New guest registered. Number: %d\n",countGuests()+1);
	
	
	fclose(f);	
	return;
}

int listGuests()
{
	//getchar(); //getting rid of buffer from menu
	
	if (countGuests()==0) 
	{
		printf("\nNo guests registered so far.\n\n");
		return;
	}
	
	printf("Listing guests..\n");
	printf("_____________________________\n");
	
	FILE * f;
	f=fopen("guests.dat","rb"); //read binary	
	if (f==NULL){perror("Error: Guest file not found. Maybe there are no guests.\n"); return;}
	
	guest g;	
	
	fread(&g,sizeof(g),1,f); //use fwrite for writing
	
	while (!feof(f))
	{	//printing struct		
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
	
	
	FILE * f;
	f=fopen("guests.dat","rb");
	if (f==NULL)
	{
		//perror("File opening error\n"); exit(1);
		return 0; //no file means 0 guests
	}
	
	guest g;
	fread(&g,sizeof(g),1,f);
	
	while (!feof(f)){
		i++;
		fread(&g,sizeof(g),1,f); 	
	} 	

	fclose(f);	
	return i;
}

int countAttendants(int event)
{
	int i=0; //counting guests		
	
	FILE * f;
	f=fopen("guests.dat","rb");
	if (f==NULL)
	{
		//perror("File opening error\n"); exit(1);
		return 0; //no file means 0 guests
	}
	
	guest g;
	fread(&g,sizeof(g),1,f);
	
	while (!feof(f)){
		if (g.eventID==event)
		{
			i++;
		}		
		fread(&g,sizeof(g),1,f); 	
	} 	

	fclose(f);	
	return i;	
}

int updateGuest()
{	
	getchar(); //getting rid of buffer from menu
	
	if (countGuests()==0) 
	{
		printf("\nNo guests registered so far.\n\n");
		return;
	}
	
	int rc;
    char buff[80];
	char target[80];	
	
	guest g;
	guest g_upd;	
	
	FILE * f;
	f=fopen("guests.dat","rb+"); //update binary
	if (f==NULL){perror("File opening error\n"); exit(1);}
	
	//rc = getLine ("Please enter the name of the new guest: \n", buff, sizeof(buff));
	//getchar();
	//strcpy(g.name, buff);
	
	//printf("Please enter the number of the guest you want to update: \n");
	rc = getLine ("Please enter the name of the guest you want to update: \n", target, sizeof(target));
	
	rc = getLine ("Please enter the new name or 0 if you dont wish to change it:\n", buff, sizeof(buff));
	strcpy(g_upd.name, buff);
	
	rc = getLine ("Please enter the new email address or 0 if you dont wish to change it:\n", buff, sizeof(buff));
	strcpy(g_upd.email, buff);
	
	printf("Please enter the new ID of the event or 0 if you dont wish to change it: \n");
	scanf("%d", &g_upd.eventID);
	
	int i=0;
	while (!feof(f))
	{
		fread(&g,sizeof(g),1,f); 		
		if (strcmp(g.name,target)==0) //if we find the specified name
		{			
			//we fill the record			
			if (strcmp(g_upd.name,"0")==0)
			{
				strcpy(g_upd.name,g.name);
			}
			
			if ((strcmp(g_upd.email,"0")==0))
			{
				strcpy(g_upd.email,g.email);
			}
			
			if (g_upd.eventID==0)
			{
				g_upd.eventID=g.eventID;
			}
			
			strcpy(g_upd.regdate,g.regdate);
			//record is filled
			
			//we update the guest data
			
			//first we reposition			
			fseek(f, i*sizeof(guest),SEEK_SET);
			
			//fwrite(&g_upd, sizeof(guest), sizeof(g_upd), f);
			fwrite(&g_upd, sizeof(guest), 1, f);
			
			printf("Guest data updated.\n");
			fclose(f);
			return;
			
		} //else we keep looking
		//else printf("test notfound %d\n",i);
		i++;
	} 
	
	printf("We could not find the guest you specified.\n");		
	
	fclose(f);	
	printf("Press any key to continue..\n");
	return;
}

int delGuest()
{		
	getchar(); //getting rid of buffer from menu
	
	if (countGuests()==0) 
	{
		printf("\nNo guests registered so far.\n\n");
		return;
	}
	
	int rc;
    char buff[80];
	char target[80];	
	
	guest g;	
	
	FILE * f;
	f=fopen("guests.dat","rb+"); //update binary
	if (f==NULL){perror("File opening error\n"); exit(1);}	
	
	FILE * f_temp;
	f_temp=fopen("guests2.dat","wb"); //write binary
	if (f==NULL){perror("File opening error\n"); exit(1);}	
	
	rc = getLine ("Please enter the name of the guest you want to delete: \n", target, sizeof(target));
	
	fread(&g,sizeof(g),1,f); 	
	while (!feof(f)) //read the original file
	{		
		if (strcmp(g.name,target)!=0) //if its not the specified name
		{
			fwrite(&g, sizeof(guest), 1, f_temp); //record into file			
		}
		fread(&g,sizeof(g),1,f); 		
	}
	
	rename("guests.dat","guests_old.dat");
	rename("guests2.dat","guests.dat");
	
	printf("Guest deleted.\n");
	
	fclose(f);
	fclose(f_temp);
	
	printf("Press any key to continue..\n");
	
	return;
}

int addEvent()
{	
	getchar(); //getting rid of buffer from menu
	
	FILE * f;
	f=fopen("events.dat","ab"); //append binary
	if (f==NULL){perror("File opening error\n"); exit(1);}
	
	event e;	
	
	int rc;
    char buff[80];
    
	rc = getLine ("Please enter the name of the new event: \n", buff, sizeof(buff));
	
	strcpy(e.name, buff);
	
	rc = getLine ("Please enter the location of the event: \n", buff, sizeof(buff));
	
	strcpy(e.location, buff);	
	
	printf("Please enter the date of the event: \n");
	rc = getLine ("(Day Mon DD  HH:MM:SS YYYY) \n", buff, sizeof(buff));
	
	strcpy(e.eventdate, buff);
	
	printf("Please enter the ID of the event: \n");
	scanf("%d", &e.ID);	
	
	//reg date	
	/*
	time_t curtime;
    time(&curtime); 
	
	strcpy(g.regdate,ctime(&curtime));	
	
	printf("Date of registration: %s\n",g.regdate);
	*/
	
	//todo fix
	//strcpy(e.eventdate,"1999.99.99");
	
	fwrite(&e, sizeof(event), 1, f); //record into file		
	
		
	fclose(f);	
	printf("Event added.\n");
	
	return;
}


int listEvents()
{
	printf("Listing events..\n");
	printf("_____________________________\n");
	
	FILE * f;
	f=fopen("events.dat","rb"); //read binary	
	if (f==NULL){perror("Error: Event file not found. Maybe there are no events.\n"); return;}
	
	event e;	
	
	fread(&e,sizeof(e),1,f); //use fwrite for writing
	
	while (!feof(f))
	{	//printing struct		
		printf("%s | ",e.name);
		printf("%s | ",e.location);
		printf("%d | ",e.ID);
		printf("%s\n",e.eventdate);
		
		fread(&e,sizeof(e),1,f); 
	} 
	
	printf("\n___End of list.___\n\n");
	fclose(f);	
	return;
}

int countEvents()
{
	int i=0; //counting guests	
	
	
	FILE * f;
	f=fopen("events.dat","rb");
	if (f==NULL)
	{
		//perror("File opening error\n"); exit(1);
		return 0; //no file means 0 guests
	}
	
	event e;
	fread(&e,sizeof(e),1,f);
	
	while (!feof(f)){
		i++;
		fread(&e,sizeof(e),1,f); 	
	} 	

	fclose(f);	
	return i;
}

int delEvent()
{	
	getchar(); //getting rid of buffer from menu
	
	int rc;
    char buff[80];
	//char target[80];	
	int target;
	
	event e;	
	
	FILE * f;
	f=fopen("events.dat","rb+"); //update binary
	if (f==NULL){perror("File opening error\n"); exit(1);}	
	
	FILE * f_temp;
	f_temp=fopen("events2.dat","wb"); //write binary
	if (f==NULL){perror("File opening error\n"); exit(1);}	
	
	//rc = getLine ("Please enter the ID of the event you wish to delete:\n", target, sizeof(target));
	printf("Please enter the ID of the event you wish to delete:\n");
	scanf("%d", &target);
	
	fread(&e,sizeof(e),1,f); 	
	while (!feof(f)) //read the original file
	{		
		if (e.ID!=target) //if its not the specified name
		{
			fwrite(&e, sizeof(event), 1, f_temp); //record into file			
		}
		fread(&e,sizeof(e),1,f); 		
	}
	
	rename("events.dat","events_old.dat");
	rename("events2.dat","events.dat");
	
	printf("Event deleted.\n");
	
	fclose(f);
	fclose(f_temp);
	
	printf("Press any key to continue..\n");
	
	//test
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
	
	return;
}

int loadSamples()
{
	int srcFile = open("_guests.dat", O_RDONLY, 0);
	int dstFile = open("guests.dat", O_WRONLY | O_CREAT, 0750);
	
	struct stat source_file_state;
	
	fstat(srcFile, &source_file_state);
	sendfile(dstFile, srcFile, 0, source_file_state.st_size);
	
	close(dstFile);
	close(srcFile);	
	
	//2.
	
	srcFile = open("_events.dat", O_RDONLY, 0);
	dstFile = open("events.dat", O_WRONLY | O_CREAT, 0750);
	
	//struct stat source_file_state;
	
	fstat(srcFile, &source_file_state);
	sendfile(dstFile, srcFile, 0, source_file_state.st_size);
	
	close(dstFile);
	close(srcFile);	
	
	
	printf("Loaded default files.\n");
	
	return;
}

int handleEvents() 
{	
	
	int pipefd[2]; // unnamed pipe file descriptor array
	pid_t pid;
    char buf[200];  // char array for reading from pipe
	char cp[50];
	char str[80];
	
	signal(SIGTERM,handler);

    int readpipe[2];
	int writepipe[2];
	int a;
	int b;
	a=pipe(readpipe);
	b=pipe(writepipe);
	if (a == -1) { perror("pipe"); exit(EXIT_FAILURE); }
	if (b == -1) { perror("pipe"); exit(EXIT_FAILURE); }
          
	
	pid=fork(); //forks make a copy of variables
	
	if (pid == -1) 
	{
        perror("Fork hiba");
        exit(EXIT_FAILURE);
    }
	
	if (pid<0){perror("The fork calling was not succesful\n"); exit(1);} 
	if (pid>0) //the parent process, it can see the returning value of fork - the child variable!
	{  //parent proc, COMPANY
	
		//piping
		close(readpipe[0]);
        close(writepipe[1]);
		
		// we assume events are ordered by date		
		
		int n=countEvents();		
		if (n==0) 
		{
			perror("No events. We have nothing to do.");
		}
		
		
		
		FILE * f;
		f=fopen("events.dat","rb"); //read binary	
		if (f==NULL){perror("Error: Event file not found. Maybe there are no events.\n"); return;}
		
		printf("\n[COMPANY] Starting..");
		
		event e;	
		
		fread(&e,sizeof(e),1,f); 		
		
		while (!feof(f))
		{	/*		
			printf("%s | ",e.name);
			printf("%s | ",e.location);
			printf("%d | ",e.ID);
			printf("%s\n",e.eventdate);
			*/
			
			/*event handling inside loop*/
			
			//todo inloop send event to pipe
			printf("\n[COMPANY] Current event: %s",e.name);
			
			write(writepipe[0],e.name,strlen(e.location));
			
			//waiting for a signal from child
			pause(); 
			
			//we send the number of guests that signed up for the current event
			//write(writepipe[0],scountAttendants(e.ID),sizeof(countAttendants(e.ID)));
			sprintf(str,"%d",countAttendants(e.ID));
			write(writepipe[0],str,sizeof(str));
			
			
			//todo ininloop read guests, send guests to pipe
		
			//todo inloop receive report+list from child, print
		
			//todo loop goes on
			
			
			fread(&e,sizeof(e),1,f); 
			
		} 			
		
		fclose(f);				
	
		/*
		waitpid(child,&status,0); 
		//waits the end of child process PID number=child, the returning value will be in status
		//0 means, it really waits for the end of child process - the same as wait(&status)
		*/
				
	}
	else //child process
	{  //child proc, ORGANIZER
		/*
		notacommonvalue=5; //it changes the value of the copy of the variable
		printf("The value is %i in child process \n",notacommonvalue);*/		
		
		//piping
		close(readpipe[1]);
        close(writepipe[0]); 			

		printf("\n[ORGANIZER] Starting..");
		
			
			if(read(readpipe[0],buf,sizeof(buf)) < 0)
			{				
				//break;
			}
			else
			{
				//guest list
				int listSize=30;
				char *guestlist[listSize];
				int gc=0;
				int i=0;
				srand(time(NULL));
				int r = rand();  
				
				while (1==1)
				{	
				
					// event location
					printf("\n[ORGANIZER] I have traveled to: %s. Waiting for guests.",buf);
					sleep(2);
					
					//todo signal..
					kill(getppid(),SIGUSR1);
					
					//write(writepipe[1],"ready",6);
					//write(writepipe[1],"continue",9);
					
					//read guestcount
					read(readpipe[0],buf,sizeof(buf));
					gc = atoi(buf);
					printf("\n[ORGANIZER] Debug: %d",gc);
					write(writepipe[1],"continue",9);
					
					
					//read guests
					printf("\n[ORGANIZER] Listing guests..");					
					
					
					for (i=0; i<gc; i++)
					{
						read(readpipe[0],buf,sizeof(buf));
						guestlist[i] = buf;
						printf("\n[ORGANIZER] %d %s",i,guestlist[i]);					
					}					
					
					printf("\n[ORGANIZER] The event is being held.");		
					sleep(2);					
					
					//write(writepipe[1],"continue",9);					
			
					//todo rate event RND
					r = rand()%100+1;  
					//write(writepipe[1],r,sizeof(r));
					sprintf(str,"%d",r);
					write(writepipe[1],str,sizeof(str));
					
					//todo inloop guests 10% RND
					for (i=0;i<gc;i++)
					{
						r = rand()%100+1;  
						if (r<10) 
						{
							//todo missing guest
							//write(writepipe[1],str,sizeof(str));
						}
					}
					
					//write finished missing
			
					//todo, finished, loop continues until no more events
					
					//exiting if this was the last event
					if (buf=="exit")
					{
						break;
					}	//else it was a location..
				} //end of single event cycle
			}
		
	}	
	
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
			//list events
			listEvents();
		break;
		case '7':
			//del event
			delEvent();
		break;
		case '8':
			//start event simulation
			//handleEvents();
			//testing
			printf("\n%d",countEvents());
		break;		
		case '9':
			//load samples
			loadSamples();
		break;
		default:
			printf("Bad input. Please choose from 0 - 8.\n");			
	}
	
	//getchar(); //consume the newline
	/*while((c = getchar()) != '\n' && c != EOF)
		/* discard */ ;
	int q;
	while ((q = getchar()) != '\n' && q != EOF);	

	printMenu();
	
	c = getchar();
	
} while (!isspace(c));  
 
 return 0;
}