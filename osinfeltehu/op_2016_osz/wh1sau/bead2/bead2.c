#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <signal.h>

struct Visitor
{
	char Name[30];
	char Email[30];
	int EventId;
	time_t RegisterDate;
	
};

struct Event
{	
	char Name[30];
	int Id;
};

void getInput(char* text, int inputSize, int minimumSize, char* output)
{	
	char buffer[inputSize+1];
	int isValid = 0;
	do
	{	
	printf("%s",text);
	fgets(buffer,inputSize+1,stdin);
	if(strlen(buffer)<minimumSize+1)
	{
		printf("Too short!\n");
		
	}
	else if(buffer[strlen(buffer)-1] != '\n')
		{
			int dropped =0;
			printf("Too long!\n");		
			while(fgetc(stdin) != '\n')
				dropped++;
			printf("%d excess characters found",dropped);
		}	
	else isValid = 1;		
	}while(!isValid);
	buffer[strcspn(buffer, "\n")] = '\0';
	strcpy(output,buffer);
}



void ListVisitors(struct Visitor* array, int number)
{	
	//	char buffer[26];
	//	struct tm* tm_info;
	int i;
	for(i=0; i<number; i++)
	{	
	//		tm_info = localtime(&array[i].RegisterDate);
	//		strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
	//		printf("%d. Visitor:\n Name: %s\tEmail:%s\tNumber:%d\tDate: %s\n",i+1,array[i].Name,array[i].Email,array[i].EventId,buffer);
			printf("%d. Visitor: Name: %-30s\tEvent id: %d\n",i+1,array[i].Name,array[i].EventId);
	}
}

void ListEvents(struct Event* array, int number)
{
	int i;
	for(i=0; i<number; i++)
	{
			printf("Event name: %-30s\tEvent Id: %d\n",array[i].Name,array[i].Id);
//			printf("%d. Visitor: Name: %s\n",i+1,array[i].Name);
	}
	
}

void AddNewVisitor(struct Visitor* array, int* index, struct Event* events, int eventCount)
{
	struct Visitor* visitor = &(array[*index]);
	getInput("Visitor name: ", 30, 1, (*visitor).Name);
	//scanf("%[^\n]%*c", &buffer);
	getInput("Visitor e-mail: ", 30, 1, (*visitor).Email);
	int isExistingId=0;
	while(!isExistingId)
	{
		printf("\nThe available events are: \n");
		ListEvents(events,eventCount);
		printf("Event Id: ");
		scanf("%d",&(*visitor).EventId);
		fgetc(stdin);
		int i=0;
		while(i<eventCount && events[i].Id != visitor->EventId)
			i++;
		if(i==eventCount)
			printf("Event with id %d does not exist!\n",i);
		else
			isExistingId=1;
	}
	time(&visitor->RegisterDate);
		(*index)++;
		char buffer[26];
		struct tm* tm_info;
		tm_info = localtime(&visitor->RegisterDate);
		strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
		printf("Visitor '%s' registered as visitor #%d at %s\n",array[*index-1].Name,*index,buffer);
		
}

void AddNewEvent(struct Event* array, int* index)
{
	struct Event* event = &(array[*index]);
	getInput("Event name: ",30,1,(*event).Name);
	int isNewId=0;
	while(!isNewId)
	{
		printf("Event Id: ");
		scanf("%d",&(*event).Id);
		fgetc(stdin);
		int i=0;
		while(array[i].Id != event->Id)
			i++;
		if(i==*index)
			isNewId=1;
		else
			printf("Id already in use!\n");
	}
	(*index)++;
	printf("Event '%s' created with Id of %d\n",event->Name,event->Id);
}

void ModifyVisitor(struct Visitor* visitor)
{
	char inputBuffer[30];
	getInput("Visitor's new name (leave blank to keep current name): ", 30, 0, inputBuffer);
	if(strlen(inputBuffer)>0)
		strcpy((*visitor).Name,inputBuffer);
	getInput("Visitor's new Email (leave blank to keep current name): ", 30, 0, inputBuffer);
	if(strlen(inputBuffer)>0)
		strcpy((*visitor).Email,inputBuffer);
	int number;
	printf("New Event number (type 0 to keep current): ");
	scanf("%d",&number);
	if(number!=0)
		(*visitor).EventId=number;	
}

void DeleteVisitor(struct Visitor* visitors, int* visitorNumber, int visitorId)
{	
	while (visitorId<*visitorNumber-1)
	{
		visitors[visitorId] = visitors[visitorId+1];
		visitorId++;
	}
	(*visitorNumber)--;
}

void DeleteEvent(struct Event* events, int* eventCount, struct Visitor* visitors, int* visitorCount)
{	
	ListEvents(events, *eventCount);
	printf("Give the ID of the event you wish to delete: ");
	int eventId=0;
	scanf("%d",&eventId);
	int i=0;
	while(i<*eventCount && events[i].Id != eventId)
			i++;
	if(i==*eventCount)
		printf("Invalid Id!\n");
	else
	{
		int visitorId=0;
		while(visitorId<*visitorCount)
		{
			if(visitors[visitorId].EventId == eventId)
				DeleteVisitor(visitors,visitorCount,visitorId);
			else
				visitorId++;
		}
		while (eventId<*eventCount-1)
		{
			events[eventId] = events[eventId+1];
			eventId++;
		}
		(*eventCount)--;
	}
}

void handler(int signo)
{
	if(signo==SIGUSR1)		
		printf("Child arrived to event, waiting for visitor list.\n");
}


void startEvent(struct Event* events, int* eventCount, struct Visitor* visitors, int* visitorCount){
setvbuf(stdout, NULL, _IONBF, 0);
	
	int pipfd1[2];
	int pipfd2[2];
	
	pid_t pid;
	struct Event currentEvent;

	if (pipe(pipfd1) == -1 || pipe(pipfd2) == -1)
	{							
		perror("Error on creating pipe!");
		exit(EXIT_FAILURE);
	}
	
	signal(SIGUSR1,handler);
	
	pid = fork();

	if (pid == -1) {
		perror("Error on Forking (Fork you!)");
		exit(EXIT_FAILURE);
	}

	//child
	if (pid == (pid_t) 0) {
		close(pipfd1[1]);
		close(pipfd2[0]);
				
				int eventId;
		read(pipfd1[0], &eventId, sizeof(eventId));
		currentEvent = events[eventId];
		printf("Child recieved event data, for event '%s'\n",currentEvent.Name);
		
		sleep(1);
		kill(getppid(),SIGUSR1);
		
		int visitorNumber;
		read(pipfd1[0], &visitorNumber, sizeof(visitorNumber));
		printf("Child recieved number of visitors, %d visitors are expected to attend the event\n", visitorNumber);
		
		struct Visitor visitors[visitorNumber];
		read(pipfd1[0], &visitors, sizeof(visitors));
		printf("Visitors have arrived to child, starting event.\n");
		
		srand(time(NULL));
		int i;
		int absent=0;
		for(i=0; i<visitorNumber; i++)
		{
			int rnd = rand() % 10;
			if(rnd==0)
			{
				absent++;
				printf("%s has not come to the event.\n",visitors[i].Name);
			}
			else
				printf("%s has come to the event.\n",visitors[i].Name);
				
		}
				
		write(pipfd2[1],&absent,sizeof(absent));
		printf("Child has finished the event\n");
		
		exit(EXIT_SUCCESS);
	}

	//parent
	else {
		close(pipfd1[0]);
		close(pipfd2[1]);
		
		int eventId;
		int isExistingId=0;
		int i = 0;
		while(isExistingId == 0){	
			printf("Event id: ");		
			scanf("%d",&eventId);
			fgetc(stdin);
					i=0;
			while(i<*eventCount && events[i].Id != eventId)
				i++;
			if(i==*eventCount)
				printf("Event with id %d does not exist!\n",i);
			else
				isExistingId=1;
		}
		
		write(pipfd1[1], &i, sizeof(i) );
		printf("Parent has sent event data.\n");
		
		int currentVisitors=0;
		for(i=0; i< *visitorCount; i++)
		{
			if(visitors[i].EventId == eventId)
				currentVisitors++;
		}
		
		pause();
		
		write(pipfd1[1], &currentVisitors , sizeof(currentVisitors) );
		printf("Parent has sent visitor number.\n");
		
		int k=0;
		struct Visitor visitorList[currentVisitors];
		for(i=0; i< *visitorCount; i++)
		{
			if(visitors[i].EventId == eventId)
			{	
				visitorList[k]=visitors[i];
				k++;
			}
		}
		
		sleep(1);
		
		write(pipfd1[1], visitorList , sizeof(visitorList) );
		printf("Parent has sent visitors to the event.\n");
		
		read(pipfd2[0], &currentVisitors, sizeof(currentVisitors));
		printf("The event has ended, %d visitors were absent during the event\n", currentVisitors);
		
	}	
}


int main()
{
	int VisitorNumber=0;
	struct Visitor visitorArray[50];
	
	int index=0;
	FILE* visitors;
if(visitors	= fopen("visitors.txt","r"))
{
	fscanf(visitors,"%d\n",&VisitorNumber);
	
	while(index<VisitorNumber)
	{
		struct Visitor* vp = &(visitorArray[index]);
	fscanf(visitors,"%[^\t]%*c\t%s\t%d\t%d\n",&(vp->Name),&(vp->Email),&(vp->EventId),&(vp->RegisterDate));
		index++;
	}
	
	fclose(visitors);
}		
	index =0;
	
	FILE* events;
	struct Event eventArray[30];
	int EventNumber=0;	
	if(events = fopen("events.txt","r"))
	{
	fscanf(events,"%d\n",&EventNumber);
	
	while(index<EventNumber)
	{
		struct Event* ep = &(eventArray[index]);
	fscanf(events,"%[^\t]%*c\t%d\n",&(ep->Name),&(ep->Id));
		index++;
	}
	fclose(events);
	}
	int response;
	
	system("clear");
do{	
	char* menuTitle = "\nKing of Stands blhblah system\n";
	char* menuOptions[5];
	menuOptions[0]= "1. Register visitor\n";
	menuOptions[1]="2. List visitors\n";
	menuOptions[2]="3. Modify/Delete visitor data\n";
	menuOptions[3]="4. List Events\n";
	menuOptions[4]="5. Add new event\n";
	menuOptions[5]="6. Delete event data\n";
	menuOptions[6]="7. Start Event\n";
	printf("%s",menuTitle);
	int i;
	for(i=0; i<7; i++)
	{
		printf("%s",menuOptions[i]);
	}
	printf("(Press 0 to exit)\n");
	scanf(" %d",&response);
	getchar();
	if(response!=0)
	{
	
	system("clear");
	switch(response-1)
	{
		case 0 : AddNewVisitor(visitorArray, &VisitorNumber, eventArray,EventNumber);break;
		case 1 : printf("List visitors:\n");  ListVisitors(visitorArray, VisitorNumber); break;
		case 2 :
			printf("Give the ID of the visitor you wish to modify/delete: ");
			int visitorId=0;
			scanf("%d",&visitorId);
			if(visitorId < 1 || visitorId>VisitorNumber)
				printf("invalid ID!");
			else
			{
				visitorId--;
				struct Visitor* v = &visitorArray[visitorId];
				printf("\nVisitor #%d:\nName: %s\nE-mail: %s\nEvent ID: %d\n",visitorId+1,(*v).Name, (*v).Email, (*v).EventId);
				printf("\n1. Modify\n2. Delete\n");
				int choice;
				scanf("%d",&choice);
				fgetc(stdin);
				switch(choice)
				{
					case 1: ModifyVisitor(v); break;
					case 2: DeleteVisitor(visitorArray,&VisitorNumber,visitorId); break;
					default: printf("Invalid choice");
				}
				
			}
			break;
		case 3 : printf("List events:\n"); ListEvents(eventArray, EventNumber); break;
		case 4 : AddNewEvent(eventArray,&EventNumber);break;		
		case 5 : DeleteEvent(eventArray, &EventNumber, visitorArray, &VisitorNumber); break;
		case 6 : startEvent(eventArray, &EventNumber, visitorArray, &VisitorNumber); break;
		default : printf("Invalid choice!");
	}
	}
}while(response);

visitors=fopen("visitors.txt","w");
fprintf(visitors,"%d\n",VisitorNumber);
for(index=0; index<VisitorNumber; index++)
{
	struct Visitor* vp = &(visitorArray[index]);
	fprintf(visitors,"%s\t%s\t%d\t%d\n",vp->Name,vp->Email,vp->EventId,vp->RegisterDate);
}
fclose(visitors);
events = fopen("events.txt","w");
fprintf(events,"%d\n",EventNumber);
	for(index=0;index<EventNumber;index++)
	{
		struct Event* ep = &(eventArray[index]);
	fprintf(events,"%s\t%d\n",ep->Name,ep->Id);
		
	}
	fclose(events);
}