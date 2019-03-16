#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>


int globalInt = 0;

typedef struct Guest {
    char* name;
    char* mail;
    int eventID;
	int autoID;
	char* date;
} Guest;

typedef struct Event {
    char* name;
    int ID;
} Event;

struct Datas {
    Guest* guests;
    int guestSize;
    Event* events;
    int eventSize;
};

char *DateTime()
{
    char *array = (char*)malloc(sizeof(char)*25);
    time_t result;
    result = time(NULL);
    sprintf(array, "%s", asctime(localtime(&result)));
    array[25] = '\0';
    return array;
}

char* Read()
{

    char* buffer = malloc(100);
    fgets(buffer, 100, stdin);
    size_t ln = strlen(buffer)-1;
    if (buffer[ln] == '\n')
        buffer[ln] = '\0';
    char* ptr;
    ptr = buffer;

    return ptr;
}

void WriteFile(struct Datas* data)
{
	FILE *fp;
    fp = fopen("datas.txt", "w");
	
	fprintf(fp, "%d", data->guestSize);
	fprintf(fp, "%s", "\n");
	int i;
	for(i=0; i < data->guestSize; ++i)
	{
		fprintf(fp,"%s",data->guests[i].name);
		fprintf(fp,"%s"," ");
		fprintf(fp,"%s",data->guests[i].mail);
		fprintf(fp,"%s"," ");
		fprintf(fp,"%d",data->guests[i].eventID);
		fprintf(fp,"%s"," ");
		fprintf(fp,"%d",data->guests[i].autoID);
		fprintf(fp,"%s"," ");
        fprintf(fp,"%s",data->guests[i].date);
	}
	
	fprintf(fp, "%s", "###");
	fprintf(fp,"%s","\n");
	
	fprintf(fp, "%d", data->eventSize);
	fprintf(fp, "%s", "\n");
	for(i=0; i < data->eventSize; ++i)
	{
		fprintf(fp,"%s",data->events[i].name);
		fprintf(fp,"%s"," ");
		fprintf(fp,"%d",data->events[i].ID);
        fprintf(fp,"%s","\n");
	}
	
    fclose(fp);
}

void ReadFile(struct Datas* data)
{
	
   FILE *fp;
   char* buffer = malloc(100);
  
   fclose(fp);
}

void AddNewGuest(struct Datas* data)
{
    char enter[1];
    gets( enter );

    struct Guest* guestPtr;
    guestPtr = (struct Guest*) malloc(sizeof(struct Guest));

    printf("Enter the name of the Guest:\n");
    guestPtr->name = Read();
    printf("Enter the mail of the Guest:\n");
    guestPtr->mail = Read();
    printf("Enter the eventID of the Guest:\n");
    guestPtr->eventID = atoi(Read());
	guestPtr->autoID = globalInt;
	guestPtr->date = DateTime();
	globalInt = globalInt + 1;
	
	printf("Jelenlegi gsize: %d",data->guestSize);
	
	bool found = false;
	int i;
	for(i=0; i < data->eventSize; ++i)
    {
        if(data->events[i].ID == guestPtr->eventID)
		{
			found = true;
		}
    }
	if(found)
	{	
		data->guestSize = data->guestSize + 1;
		data->guests = realloc(data->guests, data->guestSize * sizeof(*(data->guests)));
		data->guests[data->guestSize-1] = *guestPtr;
		WriteFile(data);
	}
	else
	{
		printf("There is no event with this ID.\n");
	}
}

void ReadGuestList(struct Datas* data)
{
    int i;
    for(i=0; i < data->guestSize; ++i)
    {
        printf("%s",data->guests[i].name);
        printf(" ");
        printf("%s",data->guests[i].mail);
        printf(" ");
        printf("%d",data->guests[i].eventID);
		printf(" ");
		printf(" ");
		printf("%d",data->guests[i].autoID);
		printf(" ");
        printf("%s",data->guests[i].date);
        printf("\n");
		
    }
}

void EditGuest(struct Datas* data)
{
	char enter[1];
    gets( enter );
	
	char* name;
	printf("Please type the name of the Guest\n");
	name = Read();
	int i;
    for(i=0; i < data->guestSize; ++i)
    {
	   int expr;
	   expr = strcmp(data->guests[i].name, name);
       if(expr == 0)
	   {
		   printf("Enter the modified name: \n");
		   data->guests[i].name = Read();
		   printf("Enter the modified mail: \n");
		   data->guests[i].mail = Read();
		   printf("Enter the modified name: \n");
		   data->guests[i].eventID = atoi(Read());
	   }
    }
	WriteFile(data);
}

void DeleteGuest(struct Datas* data)
{
	char enter[1];
    gets( enter );
	
	char* name;
	printf("Please type the name of the Guest\n");
	name = Read();
	struct Guest* temp;
	if(data->guestSize > 1)
	{
		temp = malloc((data->guestSize - 1) * sizeof(*(data->guests)));
	}	
	else
	{
		temp = malloc(sizeof(*(data->guests)));
	}
	
	bool found = false;
	int i = 0;
	while(i < data->guestSize)
	{	
		if(strcmp(data->guests[i].name,name) == 0)
		{
			found = true;
			i++;
		}
		if(found)
		{
			temp[i-1].name = data->guests[i].name;
			temp[i-1].mail = data->guests[i].mail;
			temp[i-1].eventID = data->guests[i].eventID;
			temp[i-1].autoID = data->guests[i].autoID;
			temp[i-1].date = data->guests[i].date;
		}
		else
		{
			temp[i].name = data->guests[i].name;
			temp[i].mail = data->guests[i].mail;
			temp[i].eventID = data->guests[i].eventID;
			temp[i].autoID = data->guests[i].autoID;
			temp[i].date = data->guests[i].date;
		}
		i++;
	}
	if(found)
	{
		free(data->guests);
		data->guests = NULL;
		data->guests = temp;
		data->guestSize = data->guestSize - 1;
	}
	
	WriteFile(data);
}

void DeleteEvent(struct Datas* data)
{
	// 1. lépés, az ilyen eventID-jú vendégek törlése
	
	char enter[1];
    gets( enter );
	
	char* name;
	int id;
	printf("Please type the name of the Event\n");
	name = Read();
	
	struct Event* temp;
	if(data->eventSize > 1)
	{
		temp = malloc((data->eventSize - 1) * sizeof(*(data->events)));
	}	
	else
	{
		temp = malloc(sizeof(*(data->events)));
	}
	bool found = false;
	int i = 0;
	while(i < data->eventSize)
	{	
		if(strcmp(data->events[i].name,name) == 0)
		{
			id = data->events[i].ID;
			found = true;
			i++;
		}
		if(found)
		{
			temp[i-1].name = data->events[i].name;
			temp[i-1].ID = data->events[i].ID;
		}
		else
		{
			temp[i].name = data->events[i].name;
			temp[i].ID = data->events[i].ID;
		}
		i++;
	}
	if(found)
	{
		free(data->events);
		data->events = NULL;
		data->events = temp;
		data->eventSize = data->eventSize - 1;
	}
	
	/////////////////////////////////////////////////////////////////////////
	// 2. lépés, az ilyen eventID-jú vendégek törlése
	
	int counter = 0;
	struct Guest* temp2;
	
	i = 0;
	while(i < data->guestSize)
	{	
		if(data->guests[i].eventID == id)
		{
			counter++;
			i++;
		}
	}
	
	if(data->guestSize > 1)
	{
		temp2 = malloc((data->guestSize - counter) * sizeof(*(data->guests)));
	}	
	else
	{
		temp2 = malloc(sizeof(*(data->guests)));
	}
	
	bool found2 = false;
	
	i = 0;
	while(i < data->guestSize)
	{	
		if(data->guests[i].eventID == id)
		{
			found2 = true;
			i++;
		}
		if(found2)
		{
			temp2[i-1].name = data->guests[i].name;
			temp2[i-1].mail = data->guests[i].mail;
			temp2[i-1].eventID = data->guests[i].eventID;
			temp2[i-1].autoID = data->guests[i].autoID;
			temp2[i-1].date = data->guests[i].date;
		}
		else
		{
			temp2[i].name = data->guests[i].name;
			temp2[i].mail = data->guests[i].mail;
			temp2[i].eventID = data->guests[i].eventID;
			temp2[i].autoID = data->guests[i].autoID;
			temp2[i].date = data->guests[i].date;
		}
		i++;
	}
	if(found2)
	{
		free(data->guests);
		data->guests = NULL;
		data->guests = temp2;
		data->guestSize = data->guestSize - counter;
	}
	
	WriteFile(data);
}

void AddNewEvent(struct Datas* data)
{
	char enter[1];
    gets( enter );

    struct Event* eventPtr;
    eventPtr = (struct Event*) malloc(sizeof(struct Event));

    printf("Enter the name of the Event:\n");
    eventPtr->name = Read();
	
	printf("Enter the ID of the Event:\n");
    eventPtr->ID = atoi(Read());

    data->eventSize = data->eventSize + 1;
    data->events = realloc(data->events, data->eventSize * sizeof(*(data->events)));
    data->events[data->eventSize - 1] = *eventPtr;
	
	WriteFile(data);
}

void ReadEventList(struct Datas* data)
{
	int i;
    for(i=0; i < data->eventSize; ++i)
    {
        printf("%s",data->events[i].name);
        printf(" ");
		printf("%d",data->events[i].ID);
        printf("\n");
    }
}

void Menu(struct Datas* data)
{
    int number;

    printf("1. Add a new guest\n");
    printf("2. Read the current list of guests\n");
	printf("3. Edit a guest\n");
    printf("4. Delete a guest\n");
	printf("5. Add a new event\n");
    printf("6. Read the current list of events\n");
	printf("7. Delete event\n");
    printf("8. Exit\n");
    scanf("%d", &number);

    do{
        if(number == 1)
        {	
            AddNewGuest(data);
        }
        else if(number == 2)
        {
            ReadGuestList(data);
        }
		else if(number == 3)
        {
           EditGuest(data);
        }
		else if(number == 4)
        {
           DeleteGuest(data);
        }
		else if(number == 5)
        {
           AddNewEvent(data);
        }
		else if(number == 6)
        {
           ReadEventList(data);
        }
		else if(number == 7)
        {
           DeleteEvent(data);
        }
		else if(number == 8)
        {
           ReadFile(data);
        }
        else if(number == 9)
        {
            exit(1);
        }
        else
        {
            printf("Sure? Something went wrong. Please again! \n");
            scanf("%d", &number);
        }
    }while(number > 7);

}

int main()
{
    struct Datas* datasPtr;
    datasPtr = (struct Datas*) malloc(sizeof(struct Datas));
    datasPtr->guestSize = 0;
    datasPtr->eventSize = 0;

    int i = 1;
    while(i > 0)
    {
        Menu(datasPtr);
        i = i + 1;
    }

    return 0;
}
