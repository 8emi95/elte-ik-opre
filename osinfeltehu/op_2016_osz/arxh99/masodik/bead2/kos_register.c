#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

//--------------------------------------------------------------------------------------------------------
// PROGRAM LOGIC
//--------------------------------------------------------------------------------------------------------

typedef enum {false=0, true=1} bool;

struct  Applicant {
  int id;
  char name[30];
  char email[30];
  int eventId;
  char date[30];
  struct  Applicant *next;
};

struct Event {
	 int id;
	 char ename[30];
	 struct Event *next;
};

//Pointer to the first element of applicants list
struct  Applicant *applicants = 0; 
//Pointer to the first element of events list
struct Event *events = 0;

//--------------------------------------------------------------------------------------------------------
//Return the event name for an event id
char* getEventName(int eId) {
	 struct Event *event = events;
	  
	 if (event != 0) {
	 
	   while ((event->next != 0) && (event->id != eId)) event = event->next;
	   return event->ename;
	 }
 
	printf("ERROR: Reading event name for eventId FAILED in function:"
				"char* getEventName(int eventId)  on line %d", __LINE__); 
	return;
}
//--------------------------------------------------------------------------------------------------------

char* getTime()
{
    char *array = (char*)malloc(sizeof(char)*25);
    time_t result;
    result = time(NULL);
    sprintf(array, "%s", asctime(localtime(&result))); 
    array[25] = '\0';
    return array;
}

//--------------------------------------------------------------------------------------------------------

int addApplication(char newName[], char newEmail[], int newEventId) {
	 int i;  
	 int newId;
	 struct  Applicant *applicant;
	
	 applicant = malloc( sizeof(struct  Applicant) );
	 applicant->next = applicants;
	 
	 for (i = 0; i < 30; i++) applicant->name[i] = newName[i];
	 for (i = 0; i < 30; i++) applicant->email[i] = newEmail[i];
	 applicant->eventId = newEventId;
	 
	 char* tmp = (char*)malloc(sizeof(char)*25);
	 tmp = getTime();
	 for (i = 0; i < 30; i++) applicant->date[i] = tmp[i];
	 free(tmp);
	 
	 if (applicants == 0) newId = 1;
	 else newId = applicants->id + 1;
	 applicant->id = newId;
	 
	 applicants = applicant; 
	 
	 return newId;
}

//--------------------------------------------------------------------------------------------------------

int loadApplication(int newId, char newName[], char newEmail[], int newEventId, char newDate[]) {
	 int i;  
	 struct  Applicant *applicant;
	
	 applicant = malloc( sizeof(struct  Applicant) );
	 applicant->next = applicants;
	 
	 for (i = 0; i < 30; i++) applicant->name[i] = newName[i];
	 for (i = 0; i < 30; i++) applicant->email[i] = newEmail[i];
	 applicant->eventId = newEventId;
	 for (i = 0; i < 30; i++) applicant->date[i] = newDate[i];
	applicant->id = newId;
	 
	 applicants = applicant; 
	 
	 return newId;
}

//--------------------------------------------------------------------------------------------------------



void addEvent(int newId, char newEname[]) {
	 struct Event *event;
	 event = malloc( sizeof(struct Event) );
	 event->next = events;
	 event->id = newId;
	 int i;
	 for (i = 0; i < 30; i++) event->ename[i] = newEname[i];
	 events = event;
}

//--------------------------------------------------------------------------------------------------------

int createEvent(char newEvent[]) {
	 int i;
	 int newId;
	 int l = 0;
	 struct Event *event = events;
	 struct Event *prev = events;
	 
	 if (event != 0) {
	 
	   while ((event->next != 0) && (l == 0)) {
		l = 1; 
		for (i = 0; i < 30; i++) if (event->ename[i] != newEvent[i]) l = 0;
		prev = event;
		event = event->next;
	   }
	   if (l == 1) return prev->id;
	 }
	 
	 if (events == 0) newId = 1;
	 else newId = events->id + 1;
	 addEvent(newId, newEvent);
	 return newId;
}



//--------------------------------------------------------------------------------------------------------
// USER INTERFACE(READ FROM/WRITE TO CONSOLE)
//--------------------------------------------------------------------------------------------------------

void listAllApplicants() {

	 printf("\nApplicants stored in the system:\n\n");
	 
	 struct Applicant *applicant = applicants;
  
	 if (applicant != 0) {
	 int i = 1;
	   do 
	   {
			printf("\n Applicant #: %d ", i);
			printf(" Id: %d |", applicant->id);
			printf(" Name: %s |", applicant->name);
			printf(" Email: %s |", applicant->email);
			printf(" Event ID: %d |", applicant->eventId);
			printf(" Event Name: %s |", getEventName(applicant->eventId));
			printf (" Time and date: %s", applicant->date );
			printf("\n-----------------");
			i++;
			applicant = applicant->next;		
	   } while (applicant != 0);
	 }
	 else printf("There are no applicants stored in the system \n");
 
}

//--------------------------------------------------------------------------------------------------------

void listApplicantsForEvent(int searchedEventId)
{
	struct Event* event = events;
	bool isResultEmpty = true;
	int i = 0;
	
	if(event != 0)
	{
		struct Applicant *applicant = applicants;
		if(applicant != 0)
		{
			while (applicant != 0) 
			{
				if(applicant->eventId == searchedEventId)
				{
					i++;
					
					printf("\n Applicant #: %d  | ", i);
					printf(" Id: %d | ", applicant->id);
					printf(" Name: %s | ", applicant->name);
					printf(" Email: %s", applicant->email);
					printf (" Time and date: %s", applicant->date );
					printf("\n-----------------\n");
					
					isResultEmpty = false;
				}
				applicant = applicant->next;
			}
			if(isResultEmpty == true)
			{
				printf("There are no applicants stored in the system for event: %s",getEventName(searchedEventId));
			}
		}
		else
		{
			printf("There are no applicants stored in the system.");
		}
		
	}
	else
	{
		//This should not happen
		printf("ERROR: Failed to list applicants for empty event list" 
			"in function void listApplicantsForEvent(int eventId) on line %d",  __LINE__ );
	}
}

//--------------------------------------------------------------------------------------------------------
int applicantNumberForEventId(int searchedEventId)
{
	struct Event* event = events;
	bool isResultEmpty = true;
	int result = 0;
	
	if(event != 0)
	{
		struct Applicant *applicant = applicants;
		if(applicant != 0)
		{
			while (applicant != 0) 
			{
				if(applicant->eventId == searchedEventId)
				{
					result++;
					isResultEmpty = false;
				}
				applicant = applicant->next;
			}
			if(isResultEmpty == true)
			{
				printf("There are no applicants stored in the system for event: %s",getEventName(searchedEventId));
			}
		}
		else
		{
			printf("There are no applicants stored in the system.");
		}
		
	}
	else
	{
		//This should not happen
		printf("ERROR: Failed to list applicants for empty event list" 
			"in function void listApplicantsForEvent(int eventId) on line %d",  __LINE__ );
	}
	
	return result;
}

//--------------------------------------------------------------------------------------------------------

void listEvents() {

	 printf("\nEvents stored in the system:\n\n");
	 
	 struct Event *event = events;
  
	 if (event != 0) {
	 int i = 1;
	   do {
		printf("ID: %d \n", event->id);
		printf("Name: %s \n", event->ename);
		printf("Applicants: \n"); listApplicantsForEvent(event->id);
		printf("\n============\n\n");
		i++;
		event = event->next;
	   } while (event != 0);
	 }
	 else printf("There are no events stored in the system \n");
 
}

//--------------------------------------------------------------------------------------------------------

bool isEventExists(eventId)
{
	struct Event* event = events;
	while(event != 0)
	{
		if(event->id == eventId) return true;
		event = event->next;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------

void insertApplication() {
	 char name[30];
	 char email[30];
	 int eventId;
	 
	 printf("\nApplication form:\n");
	 
	 printf("Your Name: ");
	 scanf("%[^\n]s", &name);
	 getchar();
	 
	 printf("\nYour Email Address: \n");
	 scanf("%[^\n]s", &email);
	 getchar();
	 
	 printf("\nThe ID of the event: \n");
	 scanf("%d", &eventId);
	 getchar();
	 
	 if(isEventExists(eventId) == true)
	 {
		int applicationId = addApplication(name, email, eventId);
		printf("\nApplication registered with the ID: %d! Thank You!\n",applicationId);
	 }
	 else
	 {
		printf("\nERROR: This event does not exists!\n");
		printf("\nApplication failed!" 
				 "Please select Menu #7 if you wish to create an event\n");
	 }
	
	 
//--------------------------------------------------------------------------------------------------------

}

void insertEvent()
{
	char eventName[30];
	printf("\nEvent creation form:\n");
	
	printf("Event Name: ");
	scanf("%[^\n]s", &eventName);
	getchar();
	int newId = createEvent(eventName);	
	printf("\nEvent successfully created!\n");
	printf("\nThe ID of your event is: %d\n", newId);
	
}

//--------------------------------------------------------------------------------------------------------
void getEventInfoById()
{
	 int eventId;
	 printf("\nThe ID of the event: \n");
	 scanf("%d", &eventId);
	 getchar();
	 
	  if(isEventExists(eventId) == true)
	 {
		printf(" Event Name: %s |", getEventName(eventId));
		printf("Applicants: \n"); listApplicantsForEvent(eventId);
		printf("\n============\n\n");
	 }
	 else
	 {
	 	 printf("\nThis event does not exist in the system.\n");
	 }
	 
	
	
}
 
 //--------------------------------------------------------------------------------------------------------
 
void modifyApplicationById() {
	 
	 listAllApplicants();
	 
	 int modifiedId;
	 printf("\nProvide the ID of the application that you wish to modify:  ");
	 scanf("%d", &modifiedId);
	 getchar();
	 
	 struct Applicant* applicant = applicants;
	 while(applicant != 0 && applicant->id != modifiedId)
	 {
		applicant = applicant->next;
	 }
	 if(applicant == 0)
	 {
		printf("\nThere is no application with the provided ID! Nothing was modified!\n");
	 }
	 else
	 {
		char newName[30];
		char newEmail[30];
		int newEventId;
		printf("\nPlease provide the new parameters for the application:\n");
		printf("Your Name: ");
		scanf("%[^\n]s", &newName);
		getchar();
		 
		printf("\nYour Email Address: \n");
		scanf("%[^\n]s", &newEmail);
		getchar();
		 
		printf("\nThe ID of the event: \n");
		scanf("%d", &newEventId);
		getchar();
		 
		int i;
		for (i = 0; i < 30; i++) applicant->name[i] = newName[i];
		for (i = 0; i < 30; i++) applicant->email[i] = newEmail[i];
		applicant->eventId = newEventId;
		
		printf("Application successfully modified!\n");
	 }
}

 //--------------------------------------------------------------------------------------------------------
 
void deleteApplicationByInputId() {

	listAllApplicants();
	 
	 int deleteId;
	 printf("\nProvide the ID of the application that you wish to delete:  ");
	 scanf("%d", &deleteId);
	 getchar();
	 
	 struct Applicant* applicant = applicants;
	 struct Applicant* prev = 0;
	 while(applicant != 0 && applicant->id != deleteId)
	 {
		prev = applicant;
		applicant = applicant->next; 
	 }
	 if(applicant == 0)
	 {
		printf("\nThere is no application with the provided ID! Nothing was deleted!\n");
	 }
	 else
	 {
		if (prev == 0)
		{
			applicants = applicant->next;
		}
		else
		{
			prev->next = applicant->next;
		}
		free(applicant);		
		printf("Application successfully deleted!\n");
	 }
}

 //--------------------------------------------------------------------------------------------------------
 
 void deleteApplicationById(int deleteId) {
	 
	 struct Applicant* applicant = applicants;
	 struct Applicant* prev = 0;
	 while(applicant != 0 && applicant->id != deleteId)
	 {
		prev = applicant;
		applicant = applicant->next; 
	 }
	 if(applicant == 0)
	 {
		printf("\nThere is no application with the provided ID! Nothing was deleted!\n");
	 }
	 else
	 {
		if (prev == 0)
		{
			applicants = applicant->next;
		}
		else
		{
			prev->next = applicant->next;
		}
		free(applicant);		
		printf("Application successfully deleted!\n");
	 }
}

 //--------------------------------------------------------------------------------------------------------
 
void deleteEventById() {

	listEvents();
	 
	 int deleteId;
	 printf("\nProvide the ID of the event that you wish to delete:  ");
	 scanf("%d", &deleteId);
	 getchar();
	 
	 struct Event* event = events;
	 struct Event* prev = 0;
	 while(event != 0 && event->id != deleteId)
	 {
		prev = event;
		event = event->next; 
	 }
	 if(event == 0)
	 {
		printf("\nThere is no event with the provided ID! Nothing was deleted!\n");
	 }
	 else
	 {
		if (prev == 0)
		{
			events = event->next;
		}
		else
		{
			prev->next = event->next;
		}
		
		struct Applicant *applicant = applicants;
		if(applicant != 0)
		{
			while (applicant != 0) 
			{
				if(applicant->eventId == deleteId)
				{
					//this frees the storage!
					deleteApplicationById(applicant->id);
				}
				applicant = applicant->next;
			}
		}
		free(event);		
		printf("Event successfully deleted!\n");
	 }
}


//--------------------------------------------------------------------------------------------------------
// FILE HANDLING(SAVING/LOADING)
//--------------------------------------------------------------------------------------------------------

void loadDataFromFile() {
	if( access("events.bin", 0 ) != -1 ) {
		FILE *eventsfile;
		eventsfile = fopen("events.bin", "r");

		if (eventsfile != NULL)
		{
			struct Event event;
			while (fread(&event, sizeof(event), 1, eventsfile)) 
			{
				addEvent(event.id, event.ename);
			}
		}
		fclose(eventsfile);

		if( access("applicants.bin", 0 ) != -1 ) {
			FILE *applicantsfile;
			applicantsfile = fopen("applicants.bin", "rb");
			if (applicantsfile != NULL) {
				struct  Applicant applicant;
				while (fread(&applicant, sizeof(applicant), 1, applicantsfile)) {
					loadApplication(applicant.id, applicant.name, applicant.email, applicant.eventId,applicant.date);
				}
			}
			fclose(applicantsfile);
		}
	}
}

//--------------------------------------------------------------------------------------------------------

void saveDataTofile() {
	 FILE *eventsfile;
	 eventsfile = fopen("events.bin", "w");
	 struct Event *event = events;
	 while (event != 0) {
		  fwrite(event, sizeof(*event), 1, eventsfile);
		  event = event->next;
	 }
	 fclose(eventsfile);
	 
	 FILE *applicantsfile;
	 applicantsfile = fopen("applicants.bin", "w");
	 struct  Applicant *applicant = applicants;
	 while (applicant != 0) {
		  fwrite(applicant, sizeof(*applicant), 1, applicantsfile);
		  applicant = applicant->next;
	 }
	 fclose(applicantsfile);
}

//--------------------------------------------------------------------------------------------------------
//  Event Organizing
//--------------------------------------------------------------------------------------------------------


static bool isCompanyOnPlace = 0;
static key_t key;

static void handleSignal(int signumber)
{
	if (signumber == SIGTERM)
	{
		isCompanyOnPlace = 1;
	}
}

void organizeEvent()
{

	int organizedEventId;
	printf("\n Which event should we organize?: ");
	scanf("%d", &organizedEventId);

	if (isEventExists(organizedEventId) == false)
	{
		printf("\n This event does not exists! \n");
		return;
	}

	struct Applicant *applicant = applicants;
	int applicantCount = applicantNumberForEventId(organizedEventId);

	struct message {
		long mtype;
		char mtext[1024];
	};

	int msgqueue = msgget(key, 0600 | IPC_CREAT);

	if (msgqueue < 0 ) {
		perror("msgget");
		exit(EXIT_FAILURE);
	}

	int pipefd[2]; // unnamed pipe file descriptor array

	pid_t cpid; // child process

	if (pipe(pipefd) == -1)
	{
		perror("Pipe opening was not succesful\n");
		exit(EXIT_FAILURE);
	}

	signal(SIGTERM, handleSignal);

	cpid = fork();
	
	if (cpid < 0) // error
	{
		perror("Fork calling was not succesful\n");
		exit(EXIT_FAILURE);
	}
	else if (cpid > 0) // the parent process, it can see the returning value of fork
	{
		pause();

		if (isCompanyOnPlace == true)
		{
			printf("\n Company arrived to event place. \n");
		}

		close(pipefd[0]); // close unused read end
		
		struct Applicant registredApplicants[applicantCount];
		int index = 0;
		struct Applicant *applicant = applicants;
		if(applicant != 0)
		{
			while (applicant != 0) 
			{
				if(applicant->eventId == organizedEventId)
				{
					registredApplicants[index] = *applicant;
					index++;
				}
				applicant = applicant->next;
			}
		}
		int i;
		for (i = 0; i < applicantCount; ++i)
		{
			char buffer[100];
			strcpy(buffer, registredApplicants[i].name);
			write(pipefd[1], &buffer, sizeof(buffer));
		}

		close(pipefd[1]); // close write descriptor

		struct message m;
		msgrcv(msgqueue, &m, 1024, 1, 0);

		printf("\n Event statistic arrived: %s\n", m.mtext);

		int status;
		waitpid(cpid, &status, 0);

		printf("\n Event finished, company freed up. New event can be organized! \n");
	}
	else // child process
	{
		sleep(2);

		kill(getppid(), SIGTERM);

		close(pipefd[1]); // close the unused write end

		sleep(2);

		printf("\nEvent starting...\n\nGuest List:\n");

		srand(time(NULL));
		int i;
		for (i = 0; i < applicantCount; ++i) {
			char buffer[100];
			read(pipefd[0], &buffer, sizeof(buffer));

			if (rand() / (RAND_MAX + 1.0) < 0.9)
			{
				printf("%s | came to the event \n", buffer);
			}
			else
			{
				printf("%s | did NOT come to the event \n", buffer);
				struct message guest = { 2, "" };
				sprintf(guest.mtext, "%s", buffer);
				msgsnd(msgqueue, &guest, strlen(guest.mtext), 0);
			}
		}

		sleep(2);
		printf("\nEvent ended\n");

		struct message review = { 1, "" };
		sprintf(review.mtext, "%d/10", rand() % 11);
		msgsnd(msgqueue, &review, strlen(review.mtext), 0);

		close(pipefd[0]); // finally we close the used read end
		exit(EXIT_SUCCESS);
	}
}


//--------------------------------------------------------------------------------------------------------
// MAIN FUNCTION / MENU
//--------------------------------------------------------------------------------------------------------

int main() {
	 loadDataFromFile();
	 printf("\n\nWelcome to King of Stands' register system\n");
	 int choice; 
	 do {
		  printf("\n\n---------------\nMenu\n\n");
		  printf("1. List ALL applications\n");
		  printf("2. List applications for a specified event ID\n");
		  printf("3. List ALL events with the applicants list\n");
		  printf("4. New Application\n");
		  printf("5. Modify an application\n");
		  printf("6. Delete an application\n");
		  printf("7. New event\n");
		  printf("8. Delete an event\n");
		  printf("9. Organize(start) an event\n");
		  
		  printf("10. Exit program");
		  printf("\n\n---------------\n");
		  scanf("%d", &choice);
		  getchar();
		  switch (choice) {
			   case 1:
				listAllApplicants();
			   break;
			   
			   	case 2:
			   getEventInfoById();
			   break;

			    case 3:
				listEvents();
			   break;
			   
			   case 4:
				insertApplication();
			   break;
			   
			   case 5:
				modifyApplicationById(); 
			   break;
			   
			   	case 6:
				deleteApplicationByInputId(); 
			   break;
			   
			   	case 7:
				insertEvent();
			   break;
			   
			   	case 8:
				deleteEventById();
			   break;
			   
			   	case 9:
				organizeEvent();
			   break;

			   case 10:
			   break;
			   default:
				printf("\n\nError: This menu does not exists\n");
			   break;
		  }
	} while (choice != 9);
	saveDataTofile();
 return 0;
}