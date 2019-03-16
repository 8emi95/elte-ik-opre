#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

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
	printf("%s",menuTitle);
	int i;
	for(i=0; i<6; i++)
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