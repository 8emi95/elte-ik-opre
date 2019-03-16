// --Dániel Őri--
// ----W5QL1A----
// ----OPRE------
// --2.HOMEWORK--

// The second homework contains some parts from the first homework, 
// but some functions and lines have been removed.
// Used technologies : signal, message, pipe, fork
// Attachments: 2 input files, (further can be generated with the first homework)

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
#include <signal.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <wait.h> 

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

void DeleteEvent(struct Datas* data, int evID)
{
	struct Event* temp;
	if(data->eventSize > 0)
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
		if(data->events[i].ID == evID)
		{
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
	

		free(data->events);
		data->events = NULL;
		data->events = temp;
		data->eventSize = data->eventSize - 1;
	
	
	WriteFile(data);
}

void ReadEventList(struct Datas* data)
{
	int i;
    for(i=0; i < data->eventSize; ++i)
    {
        printf("%s",data->events[i].name);
        printf(" ");
		printf("ID: %d",data->events[i].ID);
        printf("\n");
    }
}

int GetRandomNumber(int input)
{
   int i, number;
   time_t t;
      
   srand((unsigned) time(&t));
   number = rand() % input;
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

void handler(int signum)
{
    if (signum == SIGUSR1)
    {
        printf("Received singal!\n");
    }
}

struct message { 
     long type;
	 char percent[1024];
};

int send(int input)
{ 
	int i  = GetRandomNumber(10);
	int status;
	char buffer[1024];
	sprintf(buffer, "%d", i);
    struct message msg = { 5, buffer[0]};  

	status = msgsnd( input, &msg, strlen (msg.percent) + 1 , 0 ); 
    if ( status < 0 ) 
        perror("msgsnd"); 
    return 0; 
} 

int receive( int input ) 
{ 
     struct message msg;
     int status;
     	
	 status = msgrcv(input, &msg, 1024, 5 , 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "Parent received the percent of success: %s", msg.percent ); 
		  printf("/10 \n");
     return 0; 
} 

int main(int argc, char* argv[])
{
    struct Datas* datasPtr;
    datasPtr = (struct Datas*) malloc(sizeof(struct Datas));
    datasPtr->guestSize = 0;
    datasPtr->eventSize = 0;

	ReadFile(datasPtr);
	
	printf("Choose an event: \n");
	ReadEventList(datasPtr);	
	
	pid_t pid;
	int pipefd[2];
	int end = datasPtr->eventSize;
    char bufi1[100],bufi2[100], sizeOfNames[100];
	int status; 
	signal(SIGUSR1, handler);
	
	

	while(end > 0)
	{
		sleep(2);
		int evID = atoi(Read());
	
		int messageQue;
		key_t key;
		key = ftok(argv[0],1); 	
		messageQue = msgget( key, 0600 | IPC_CREAT ); 
		if ( messageQue < 0 ) { 
          perror("msgget"); 
          return 1; 
		} 
		
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

		if (pid == 0)  // child
		{		    	
			read(pipefd[0],bufi1, sizeof(bufi1));
			printf("Child received event:  %s", bufi1);
			
			sleep(3);
			kill(getppid(),SIGUSR1);
			printf("Child arrived waitin' for guests! Sent signal to parent\n");
					
			int size;
			read(pipefd[0], &size, sizeof(size));
			//printf("Child received the size \n%d", size);
					
			sleep(3);
			struct LunchGuest receiveArray[size];
			read(pipefd[0], receiveArray, sizeof(receiveArray));
			printf("Child received the namelist\n");

			int i;
			for(i=0; i < size; ++i)
			{
				sleep(1);
				int rum = GetRandomNumber(100);
				if( rum  > 10)
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
			//int success = GetRandomNumber();
			//write(pipefd[1], &success, sizeof(success));
			send(messageQue);
			wait( NULL ); 
            msgctl( messageQue, IPC_RMID, NULL ); 			
			printf("Child sent the percent of success\n");
			close(pipefd[1]); 
					
		} 
		else // parent
		{   
			char* eventName;
				   
			if(EventIDExist(datasPtr ,evID) > 0)
			{
				eventName= GetEventName(datasPtr, evID);
			}
			else
			{
				printf("exit\n");
				exit(1);
			}

			printf("\nParent works! \n");
			write(pipefd[1], eventName, (strlen(eventName)+1));
			printf("Parent used the pipe! (write)\n");
			sleep(2);

			pause(); // waits for the signal
			
			int size = GetSizeOfNameArray(datasPtr, evID);
				  
			write(pipefd[1], &size , sizeof(size));
			//printf("\nParent sent the size of the names\n");

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
			printf("Parent received the results: \n");
				   
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
				   
			//int success;
			//read(pipefd[0], &success, sizeof(success));
			receive(messageQue); 
			sleep(3);
			//printf("Parent received the percent of success: %d", success);
			printf("If you do not see : 'No event left' , choose an other one!\n");
			close(pipefd[0]);			
			
			DeleteEvent(datasPtr, evID);	
		}
		end--;
	}
	sleep(1);
	printf("No event left\n");
    return 0;
}
