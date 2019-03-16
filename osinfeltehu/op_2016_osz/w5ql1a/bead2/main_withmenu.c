#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

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

struct LunchGuest
{
	char* name;
	int arrived;
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

    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;)
    {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if(--len == 0)
        {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);
            if(linen == NULL)
            {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
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
		fprintf(fp,"%s","\n");
		fprintf(fp,"%s",data->guests[i].mail);
		fprintf(fp,"%s","\n");
		fprintf(fp,"%d",data->guests[i].eventID);
		fprintf(fp,"%s","\n");
		fprintf(fp,"%d",data->guests[i].autoID);
		fprintf(fp,"%s","\n");
        fprintf(fp,"%s",data->guests[i].date);
	}
	fprintf(fp, "%d", data->eventSize);
	fprintf(fp, "%s", "\n");
	for(i=0; i < data->eventSize; ++i)
	{
		fprintf(fp,"%s",data->events[i].name);
		fprintf(fp,"%s","\n");
		fprintf(fp,"%d",data->events[i].ID);
        fprintf(fp,"%s","\n");
	}

    fclose(fp);
}

void ReadFile(struct Datas* data)
{
	FILE* fp = fopen("datas.txt", "r");
    char* line = malloc(30);

	fgets(line, 30, fp);
	data->guestSize = atoi(line);

	free(data->guests);
	data->guests = NULL;
	data->guests = malloc(data->guestSize * sizeof(*(data->guests)));

	int i;
	for(i=0; i < data->guestSize; ++i)
	{
		line = malloc(30);
		fgets(line, 30, fp);
		while(line[0] == '\n' || line[0] == '\0')
		{
			line = malloc(30);
			fgets(line, 30, fp);
		}
		data->guests[i].name = line;

		line = malloc(30);
		fgets(line, 30, fp);
		while(line[0] == '\n' || line[0] == '\0')
		{
			line = malloc(30);
			fgets(line, 30, fp);
		}
		data->guests[i].mail = line;

		line = malloc(30);
		fgets(line, 30, fp);
		while(line[0] == '\n' || line[0] == '\0')
		{
			line = malloc(30);
			fgets(line, 30, fp);
		}
		data->guests[i].eventID = atoi(line);

		line = malloc(30);
		fgets(line, 30, fp);
		while(line[0] == '\n' || line[0] == '\0')
		{
			line = malloc(30);
			fgets(line, 30, fp);
		}
		data->guests[i].autoID = atoi(line);

		line = malloc(30);
		fgets(line, 30, fp);
		while(line[0] == '\n' || line[0] == '\0')
		{
			line = malloc(30);
			fgets(line, 30, fp);
		}
		data->guests[i].date = line;
		line = malloc(30);
	}


	fgets(line, 30, fp);
	data->eventSize = atoi(line);

	free(data->events);
	data->events = NULL;
	data->events = malloc(data->eventSize * sizeof(*(data->events)));

	int j;
	for(j=0; j < data->eventSize; ++j)
	{
		line = malloc(30);
		fgets(line, 30, fp);
		while(line[0] == '\n' || line[0] == '\0')
		{
			line = malloc(30);
			fgets(line, 30, fp);
		}
		data->events[j].name = line;

		line = malloc(30);
		fgets(line, 30, fp);
		while(line[0] == '\n' || line[0] == '\0')
		{
			line = malloc(30);
			fgets(line, 30, fp);
		}
		data->events[j].ID = atoi(line);

	}

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
		if(data->guestSize == 1)
            data->guests = malloc(data->guestSize * sizeof(*(data->guests)));
        else
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
        printf("\n");
        printf("%s",data->guests[i].mail);
        printf("\n");
        printf("%d",data->guests[i].eventID);
		printf("\n");
		printf("%d",data->guests[i].autoID);
		printf("\n");
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
	printf("gSize: %i\n", data->guestSize);
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
		   printf("Enter the modified id: \n");
		   data->guests[i].eventID = atoi(Read());
	   }
    }
	WriteFile(data);
}

void DeleteGuest(struct Datas* data, char* name)
{
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
	char enter[1];
    gets( enter);

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

	i = 0;
	while(i < data->guestSize)
	{
		if(data->guests[i].eventID == id)
		{
			DeleteGuest(data, data->guests[i].name);
			--i;
		}
		i++;
	}

	WriteFile(data);
}

void AddNewEvent(struct Datas* data)
{
	char enter[1];
    gets( enter);

    struct Event* eventPtr;
    eventPtr = (struct Event*) malloc(sizeof(struct Event));

    printf("Enter the name of the Event:\n");
    eventPtr->name = Read();

	printf("Enter the ID of the Event:\n");
    eventPtr->ID = atoi(Read());

    data->eventSize = data->eventSize + 1;
    if(data->eventSize == 1)
        data->events = malloc(data->eventSize * sizeof(*(data->events)));
    else
        data->events = realloc(data->events, data->eventSize * sizeof(*(data->events)));
    printf("%i", data->eventSize);
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

void Menu(struct Datas* data, pid_t childpid)
{
    int number;

    printf("1. Add a new guest\n");
    printf("2. Read the current list of guests\n");
	printf("3. Edit a guest\n");
    printf("4. Delete a guest\n");
	printf("5. Add a new event\n");
    printf("6. Read the current list of events\n");
	printf("7. Delete event\n");
    printf("8. Read from file\n");
	printf("9. Its time for a child\n");
	printf("10. Exit\n");
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
			char enter[1];
			gets( enter );

			char* name;
			printf("Please type the name of the Guest\n");
			name = Read();
			DeleteGuest(data, name);
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
          // MakeChild(childpid);
        }
        else if(number == 10)
        {
            exit(1);
        }
        else
        {
            printf("Sure? Something went wrong. Please again! \n");
            scanf("%d", &number);
        }
    }while(number > 10);

}

int GetRandomNumber()
{
   int i, number;
   time_t t;
      
   srand((unsigned) time(&t));
   number = rand() % 100;
   return number;
}

int GetSizeOfNameArray(struct Datas* data, int ID)
{	
	int result = 0;
	int i;
	for( i=0; i < data->guestSize; ++i)
	{
		if(data->guests[i].eventID == ID)
		{
			result += 1;
		}
	}
	return result;
}

char* GetEventName(struct Datas* data, int input)
{
	char* value;
	int i;
	for(i = 0; i< data->eventSize; ++i)
	{
		if(data->events[i].ID == input)
		{
			return data->events[i].name;
		}
	}
	
}

int EventIDExist(struct Datas* data, int input)
{
	int i;
	for(i = 0; i< data->eventSize; ++i)
	{
		if(data->events[i].ID == input)
		{
			return 1;
		}
	}
	return 0;
}

int main()
{
    struct Datas* datasPtr;
    datasPtr = (struct Datas*) malloc(sizeof(struct Datas));
    datasPtr->guestSize = 0;
    datasPtr->eventSize = 0;
/*
	int i = 1;
    while(i > 0)
    {
        Menu(datasPtr, childpid);
    }
	*/
		   ReadFile(datasPtr);
	
		   pid_t pid;
		   int pipefd[2],status;
           char bufi1[100],bufi2[100], sizeOfNames[100];

           if (pipe(pipefd) == -1) 
		   {
               perror("Error openin Pipe");
               exit(EXIT_FAILURE);
           }
           pid = fork();
           if (pid == -1) 
	       {
               perror("Fork err");
               exit(EXIT_FAILURE);
           }

           if (pid == 0) 
	       {		    	
				read(pipefd[0],bufi1, sizeof(bufi1));
				printf("Child received event:  %s", bufi1);
				write(pipefd[1], "OK", 3);
				printf("Child arrived waitin' for guests!\n");
				sleep(4);
				//wait(&status);
				
				int size;
				read(pipefd[0], &size, sizeof(size));
				printf("Child received the size \n%d", size);
				
				sleep(3);
				struct LunchGuest receiveArray[size];
				read(pipefd[0], receiveArray, sizeof(receiveArray));
				printf("Child received the namelist\n");

				int i;
				for(i=0; i < size; ++i)
				{
				   //printf("Child received guests: \n%s", receiveArray[i].name);
				   sleep(1);
				   int rum = GetRandomNumber();
				   if( rum  > 60)
				   {
					   receiveArray[i].arrived = 1;
				   }
				   
				   printf("%s", receiveArray[i].name);
				   printf("%d",receiveArray[i].arrived);
				   printf(" >> %d",rum);
				   printf("\n");
			    }
				
				write(pipefd[1], receiveArray, sizeof(receiveArray));
				printf("Child sent the results\n");
				
				sleep(3);
				int success = GetRandomNumber();
				write(pipefd[1], &success, sizeof(success));
				printf("Child sent the percent of success\n");
				
           } 
           else 
           {   
			   printf("Type the eventID: ");
			   int evID = atoi(Read());
			   char* eventName;
			   
			   if(EventIDExist(datasPtr ,evID) > 0)
			   {
				   eventName= GetEventName(datasPtr, evID);
			   }
			   else
			   {
				   printf("Wrong ID\n");
				   exit(1);
			   }

	           printf("\nParent works! \n");
			   printf(datasPtr->events[0].name);
               write(pipefd[1], eventName, (strlen(eventName)+1));
			   printf("Parent used the pipe! (write)\n");
			   sleep(2);
               
			   read(pipefd[0], bufi2, sizeof(bufi2));
			   printf("Parent received signal: %s", bufi2);
			   
			   int size = GetSizeOfNameArray(datasPtr, evID);
			  
			   write(pipefd[1], &size , sizeof(size));
			   printf("\nParent sent the size of the names\n");

			   sleep(3);
			   struct LunchGuest array[size];
			   int i;
			   int j = 0;
			   for(i=0; i< datasPtr->guestSize; ++i)
			   {
				   if(datasPtr->guests[i].eventID ==  evID )
				   {
					array[j].name = datasPtr->guests[i].name;
					array[j].arrived = 0;
					++j;
				   }
			   }
			   sleep(3);
			   write(pipefd[1], array, sizeof(array));
			   printf("Parent sent the names\n");
			   sleep(size*2);
			   
			   struct LunchGuest resultArray[size];
			   read(pipefd[0], resultArray, sizeof(resultArray));
			   printf("Parent received the results\n");
			   
			   for(i=0; i < size; ++i)
			   {
				   if(resultArray[i].arrived == 0)
				   {
					   printf("Not arrived: %s",resultArray[i].name);
				   }
				   else
				   {
					   printf("Arrived: %s",resultArray[i].name);
				   }
			   }
			   
			   int success;
			   read(pipefd[0], &success, sizeof(success));
			   printf("Parent received the percent of success: %d", success);
			   printf("% \n");
			   
		   }
    return 0;
}
