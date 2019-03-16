#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

// System tools
typedef enum {FALSE, TRUE} bool;
	// Pipe handling
char pipeMessage[100];


int pipes[4][2];
pid_t pids[2];

char* ReadFromPipe(int pipe)
{
    read(pipe, pipeMessage, sizeof(pipeMessage));
    fflush(NULL);
    return pipeMessage;
}

void WriteToPipe(int pipe, char *text)
{
    write(pipe, text, strlen(text) + 1);
    fflush(NULL);
}


typedef enum {FOGLALT, SZABAD, ROSSZ} status_t;
typedef enum {KOLCSONZES, VISSZAHOZAS} action_t;
typedef enum {UJPEST, OBUDA} station_t;

struct s_bike
{
	int bikeId;
	int stationId;
	status_t bikeStatus;
	struct s_bike *next;
};
typedef struct s_bike bike;

bike *firstBike = NULL;
bike *newBike = NULL;
bike *actualBike = NULL;
bike *lastBike = NULL;
int countBikes = 0;
int rentedBikesCount = 0;

struct s_event
{
	int bikeId;
	char *userName;
	action_t userAction;
	char *actionTime;
	struct s_event *next;
};
typedef struct s_event event;

event *firstEvent = NULL;
event *newEvent = NULL;
event *actualEvent = NULL;
event *lastEvent = NULL;

char* GetTime()
{
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	return asctime (timeinfo);
}

char* ConcatenateStrings(char *a, char *b)
{
	char *result = (char *) malloc(1 + strlen(a)+ strlen(b));
	strcpy(result, a);
    strcat(result, b);
	return result;
}

char* IntToStr(int number)
{
	char numberStrArray[20];
	sprintf(numberStrArray, "%d", number);
	char *numberStrPointer = (char *) malloc(1 + strlen(numberStrArray));
	strncpy(numberStrPointer, numberStrArray, (int)strlen(numberStrArray));
	return numberStrPointer;
}

int IntComparator(const void *aa, const void *bb)
{
    const int *a = aa, *b = bb;
    return (*a < *b) ? -1 : (*a > *b);
}

int GetFirstFreeId()
{
	int freeId = 0;
	if(firstBike != NULL)
	{
		if(countBikes != 1)
		{
			int bikeIds[countBikes], i = 0;
			actualBike = firstBike;
			while(actualBike != NULL)
			{
				bikeIds[i] = actualBike->bikeId;
				i++;
				actualBike = actualBike->next;
			}
			printf("\n COUNTBIKES: %d\n", countBikes);
			qsort(bikeIds, countBikes, sizeof(int), IntComparator);
			
			i = 0;
			while(freeId == 0)
			{
				if((i < countBikes - 1) && ((bikeIds[i + 1] - bikeIds[i]) > 1))
				{
					freeId = bikeIds[i] + 1;
				}
				
				if(i == countBikes - 1)
				{
					freeId = bikeIds[i] + 1;
				}
				
				i++;
			}
		}
		else
		{
			freeId = 2;
		}
	}
	else
	{
		freeId = 1;
	}
	
	printf("\n FREEID: %d\n", freeId);
	return freeId;
}

char* GetLastUserOfBike(int bikeId)
{
	char *lastUser;
	char temp[1000];
	
	actualEvent = firstEvent;
	while(actualEvent != NULL)
	{
		if(actualEvent->bikeId == bikeId && actualEvent->userAction == KOLCSONZES)
		{
			snprintf(temp, strlen(actualEvent->userName) + 1, "%s", actualEvent->userName);
		}
		actualEvent = actualEvent->next;
	}

	lastUser = (char*) calloc(1, sizeof(char) + 1);
	snprintf(lastUser, strlen(temp) + 1, "%s", temp);
	
	printf("Utolso kolcsonzo: %s", lastUser);
	return lastUser;
}

void AddEventToList(int bikeId, char *userName, action_t userAction, char *actionTime)
{
	newEvent = malloc(sizeof(event));
	newEvent->bikeId = bikeId;
	newEvent->userName = userName;
	newEvent->userAction = userAction;
	newEvent->actionTime = actionTime;
	newEvent->next = NULL;
	
	actualEvent = lastEvent;
	
	if(firstEvent == NULL)
	{
		lastEvent = actualEvent = firstEvent = newEvent;
	}
	else
	{
		actualEvent->next = newEvent;
		lastEvent = actualEvent->next;
	}
}

void AddBikeToList(int bikeId, int stationId, status_t bikeStatus)
{
	
	newBike = malloc(sizeof(bike));
	newBike->bikeId = bikeId;
	newBike->stationId = stationId;
	newBike->bikeStatus = bikeStatus;
	newBike->next = NULL;
	
	actualBike = lastBike;
	
	if(firstBike == NULL)
	{
		lastBike = actualBike = firstBike = newBike;
		countBikes++;
	}
	else
	{
		actualBike->next = newBike;
		lastBike = actualBike->next;
		countBikes++;
	}
}

void ListAllBike()
{
	if(firstBike != NULL)
	{
		actualBike = firstBike;
		while(actualBike != NULL)
		{
			printf("A bicikli azonositoja: %d\n", actualBike->bikeId);
			printf("A bicikli allomasa: %d\n", actualBike->stationId);
			
			switch (actualBike->bikeStatus)
			{
				case FOGLALT:
					printf("Bicikli allapota: FOGLALT \n++++++++++++++++++++++++++++\n");
					break;
				case SZABAD:
					printf("Bicikli allapota: SZABAD \n++++++++++++++++++++++++++++\n");
					break;
				case ROSSZ:
					printf("Bicikli allapota: ROSSZ \n++++++++++++++++++++++++++++\n");
					break;
				default:
					printf("Bicikli allapota: NEM ISMERT \n++++++++++++++++++++++++++++\n");
					break;
			}
			actualBike = actualBike->next;
		}
	}
	else
	{
		printf("A biciklik listaja ures!\n");
	}
}

void RentBike()
{
	int stationId;
	
	do
	{
		printf("\nAdd meg az allomas szamat (1-Ujpest 2-Obuda 0-Megse):\n");
		scanf("%d", &stationId);
		getchar();
	}
	while(stationId > 2 || stationId < 0);
	
	if(stationId != 0)
	{
		actualBike = firstBike;
		while(actualBike != NULL && (actualBike->stationId != stationId || actualBike->bikeStatus != SZABAD))
		{
			actualBike = actualBike->next;
		}
		
		if(actualBike != NULL)
		{
			char *userName;
			userName = (char*) calloc(1, sizeof(char));
			printf("\nKerem a felhasznalo nevet:\n");
			scanf("%[^\n]s", userName);
			getchar();
			
			int pipeId = (stationId) * 2 - 1;
			
			char *bikeIdStr, *bikeIdMessage;
			bikeIdStr = IntToStr(actualBike->bikeId);
			bikeIdMessage = (char *) malloc(strlen(bikeIdStr) + 2);
			snprintf(bikeIdMessage, strlen(bikeIdStr) + 2, "0%s", bikeIdStr);
			
			WriteToPipe(pipes[pipeId][1], bikeIdMessage);
			ReadFromPipe(pipes[pipeId - 1][0]);
			
			AddEventToList(actualBike->bikeId, userName, KOLCSONZES, GetTime());
			
			actualBike->bikeStatus = FOGLALT;
			
			rentedBikesCount++;
			
			//printf("A(z) %d szamu bicikli sikeresen ki lett berelve!\n\n", actualBike->bikeId);
		}
		else
		{
			printf("Nincs szabad bicikli a megadott allomason!\n\n");
		}
	}
}

int ReturnBike()
{
	srand((unsigned) time(NULL));;
	int stationId = (rand() % 2);
	int pipeId = ((stationId + 1) * 2 - 1);
	
	actualBike = firstBike;
	while(actualBike->bikeStatus != FOGLALT)
	{
		if(actualBike->next != NULL)
		{
			actualBike = actualBike->next;
		}
	}
	
	int i = 0;
	int temp = actualBike->bikeId;
	while(temp != 0)
	{
		temp /= 10;
		++i;
	}
	
	char *bikeIdStr, *bikeIdMessage, *message;
	bikeIdStr = (char *) malloc(temp + 2);
	snprintf(bikeIdStr, temp + 2, "%d", actualBike->bikeId);
	bikeIdMessage = (char *) malloc(strlen(bikeIdStr) + 2);
	snprintf(bikeIdMessage, strlen(bikeIdStr) + 2, "1%s", bikeIdStr);
	
	WriteToPipe(pipes[pipeId][1], bikeIdMessage);
    message = ReadFromPipe(pipes[pipeId - 1][0]);
    printf("Message: %s\n\n", message);
	AddEventToList(actualBike->bikeId, GetLastUserOfBike(actualBike->bikeId), VISSZAHOZAS, GetTime());
	switch(atoi(message))
	{
		case 0:
			actualBike->bikeStatus = ROSSZ;
			break;
		case 1:
			actualBike->bikeStatus = SZABAD;
			break;
	}
	
	actualBike->stationId = stationId + 1;
	
	return 0;
}

void ListAllBikeOnStation()
{
	char *stationIdStr;
	stationIdStr = (char*) calloc(1, sizeof(char));
	printf("\nAdd meg az allomas szamat:\n");
	scanf("%[^\n]s", stationIdStr);
	getchar();
	int stationId = atoi(stationIdStr);
	
	if(firstBike != NULL)
	{
		printf("Az allomashoz tartozo biciklik: ");
		actualBike = firstBike;
		while(actualBike != NULL)
		{
			if(actualBike->stationId == stationId)
			{
				printf("%d ", actualBike->bikeId);
			}
			
			actualBike = actualBike->next;
		}
		printf("\n\n");
	}
	else
	{
		printf("A biciklik listaja ures!\n");
	}
}

int CreateBike()
{
	char *stationIdStr;
	int stationId = 3;
	
	stationIdStr = (char*) calloc(1, sizeof(char));
	do
	{
		printf("\nAdd meg az uj bicikli allomasat (1 - Ujpest; 2 - Obuda; 0 - Megse):\n");
		scanf("%[^\n]s", stationIdStr);
		getchar();
		stationId = atoi(stationIdStr);
		if(stationId > 2)
		{
			printf("HIBA!!! Az allomas nem letezik!\n");
		}
	}
	while(stationId > 2 || stationId < 0);
	
	if(stationId != 0)
	{
		int bikeId = GetFirstFreeId();
		AddBikeToList(bikeId, stationId, SZABAD);
	}
	
	return 0;
}

int RemoveBike()
{
	char *bikeIdStr;
	bikeIdStr = (char*) calloc(1, sizeof(char));
	printf("\nAdd meg a torlendo bicikli szamat:\n");
	scanf("%[^\n]s", bikeIdStr);
	getchar();
	int bikeId = atoi(bikeIdStr);
	
	bike *previousBike = NULL;
	actualBike = firstBike;
	previousBike = firstBike;
	
	if(firstBike->bikeId == bikeId)
	{
		firstBike = firstBike->next;
	}
	else
	{
		while(actualBike->bikeId != bikeId)
		{
			if(actualBike->next != NULL)
			{
				previousBike = actualBike;
				actualBike = actualBike->next;
			}
			else
			{
				printf("Nincs ilyen bicikli!\n\n");
				return -1;
			}
		}
	}
	
	previousBike->next = actualBike->next;
	free(actualBike);
	return 0;
}

int OpenBikeFile()
{
	if(access("./OP_BIKE", F_OK) != -1)
	{
		int dataFileReadDescriptor = open("./OP_BIKE", O_RDONLY);
		if(dataFileReadDescriptor != 0)
		{
			int bikeId, stationId;
			status_t bikeStatus;
			
			char *word;
			word = (char*) calloc(1, sizeof(char));
			char buffer;
			int wordNumber = 0;

			while(read(dataFileReadDescriptor, &buffer, sizeof(buffer)) != 0)
			{
				
				if(buffer == ',')
				{
					if(wordNumber == 0)
					{
						bikeId = atoi(word);
					}
					else if(wordNumber == 1)
					{
						stationId = atoi(word);
					}
					wordNumber++;
					word = (char*) calloc(1, sizeof(char));
				}
				else if(buffer == '\n')
				{
					if(strcmp(word, "foglalt") == 0)
					{
						rentedBikesCount++;
						bikeStatus = FOGLALT;
						AddBikeToList(bikeId, stationId, bikeStatus);
					}
					else if(strcmp(word, "szabad") == 0)
					{
						bikeStatus = SZABAD;
						AddBikeToList(bikeId, stationId, bikeStatus);
					}
					else
					{
						bikeStatus = ROSSZ;
						AddBikeToList(bikeId, stationId, bikeStatus);
					}

					wordNumber = 0;
					word = (char*) calloc(1, sizeof(char));
				}
				else
				{
					char f[2] = {buffer, '\0'};
					strncat(word, f, 1);
				}
			}
			close(dataFileReadDescriptor);
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		printf("A bike fajl nem letezik\n");
		return 0;
	}
}

int WriteBikeFile()
{
	unlink("./OP_BIKE");
	int logFileAppendDescriptor = open("./OP_BIKE", O_CREAT | O_APPEND | O_WRONLY, 0644);
	
	actualBike = firstBike;
	while(actualBike != NULL)
	{
		char bikeIdArray[10], stationIdArray[10];//, *bikeIdStr, *stationId;
		
		sprintf(bikeIdArray, "%d", actualBike->bikeId);
		sprintf(stationIdArray, "%d", actualBike->stationId);

//		char *bikeIdStr = IntToStr(actualBike->bikeId);
//		char *stationIdStr = IntToStr(actualBike->stationId);
		char *status;
		
		switch(actualBike->bikeStatus)
		{
			case FOGLALT:
				status = "foglalt";
				break;
			case SZABAD:
				status = "szabad";
				break;
			case ROSSZ:
				status = "rossz";
				break;
		}
		
		char *newLine = (char *) malloc(4 + strlen(bikeIdArray) + strlen(stationIdArray) + strlen(status));
				
		sprintf(newLine, "%s,%s,%s\n", bikeIdArray, stationIdArray, status);
		
		if(logFileAppendDescriptor != -1)
		{
			if (write(logFileAppendDescriptor, newLine, strlen(newLine)) != strlen(newLine)) {
				close(logFileAppendDescriptor);
				return 0;
			}
		}
		else
		{
			close(logFileAppendDescriptor);
			return -1;
		}
		
		actualBike = actualBike->next;
	}
	close(logFileAppendDescriptor);
	return 0;
}

void ListAllEvent()
{
	if(firstEvent != NULL)
	{
		actualEvent = firstEvent;
		while(actualEvent != NULL)
		{
			printf("Az esemeny datuma: %s\n", actualEvent->actionTime);
			printf("Az erintett bicikli azonositoja: %d\n", actualEvent->bikeId);
			printf("Az esemeny vegrehajtoja: %s\n", actualEvent->userName);
			printf("Az esemeny tipusa: ");
			
			switch (actualEvent->userAction)
			{
				case KOLCSONZES:
					printf("KOLCSONZES\n++++++++++++++++++++++++++++\n");
					break;
				case VISSZAHOZAS:
					printf("VISSZAHOZAS\n++++++++++++++++++++++++++++\n");
					break;
				default:
					printf("Az esemeny tipusa: NEM ISMERT\n++++++++++++++++++++++++++++\n");
					break;
			}
			actualEvent = actualEvent->next;
		}
	}
	else
	{
		printf("A log lista ures!\n");
	}
}

int OpenLogFile()
{
	if(access("./OP_BIKE.RENT", F_OK) != -1)
	{
		int logFileReadDescriptor = open("./OP_BIKE.RENT", O_RDONLY);
		
		if(logFileReadDescriptor != 0)
		{
			
			int bikeId;
			char *userName;
			action_t userAction;
			char *actionTime;
			
			char *word;
			word = (char*) calloc(1, sizeof(char));
			char buffer[1];
			int wordNumber = 0;
			
			while(read(logFileReadDescriptor, &buffer, sizeof(buffer)) != 0)
			{
				//printf("HERE\n");
				if(buffer[0] == ',')
				{
					if(wordNumber == 0)
					{
						bikeId = atoi(word);
					}
					else if(wordNumber == 1)
					{
						userName = word;
					}
					else if(wordNumber == 2)
					{
						if(strcmp(word, "kolcsonzes") == 0)
						{
							userAction = KOLCSONZES;
						}
						else
						{
							userAction = VISSZAHOZAS;
						}
					}
					
					wordNumber++;
					word = (char*) calloc(1, sizeof(char));
				}
				else if(buffer[0] == '\n')
				{
					actionTime = (char*) malloc((int)strlen(word) + 1);
					strncpy(actionTime, word, (int)strlen(word) - 1);
					
					AddEventToList(bikeId, userName, userAction, actionTime);

					wordNumber = 0;
					word = (char*) calloc(1, sizeof(char));
				}
				else
				{
					strncat(word, buffer, 1);
				}
			}
			close(logFileReadDescriptor);
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		printf("A log fajl nem letezik!\n");
		return 0;
	}
}

int WriteLogFile()
{
	unlink("./OP_BIKE.RENT");
	int logFileAppendDescriptor = open("./OP_BIKE.RENT", O_CREAT | O_APPEND | O_WRONLY, 0644);
	//printf("LOG File desc: %d", logFileAppendDescriptor);
	
	actualEvent = firstEvent;
	while(actualEvent != NULL)
	{
		char *bikeIdStr = IntToStr(actualEvent->bikeId);
		char *action;
		
		switch(actualEvent->userAction)
		{
			case VISSZAHOZAS:
				action = "visszahozas";
				break;
			case KOLCSONZES:
				action = "kolcsonzes";
				break;
		}
		
		char *newLine = (char *) malloc(1 + strlen(bikeIdStr) + strlen(actualEvent->userName) + strlen(action) + strlen(actualEvent->actionTime));
				
		newLine = ConcatenateStrings(bikeIdStr, ",");
		newLine = ConcatenateStrings(newLine, actualEvent->userName);
		newLine = ConcatenateStrings(newLine, ",");
		newLine = ConcatenateStrings(newLine, action);
		newLine = ConcatenateStrings(newLine, ",");
		newLine = ConcatenateStrings(newLine, actualEvent->actionTime);
		if(actualEvent->next != NULL)
		{
			//newLine = ConcatenateStrings(newLine, "\n");
		}
		
		if(logFileAppendDescriptor != -1)
		{
			if (write(logFileAppendDescriptor, newLine, strlen(newLine)) != strlen(newLine))
			{
				close(logFileAppendDescriptor);
				return 0;
			}
		}
		else
		{
			close(logFileAppendDescriptor);
			return -1;
		}
		
		actualEvent = actualEvent->next;
	}
	close(logFileAppendDescriptor);
	return 0;
}

void SaveFiles()
{
	if(WriteBikeFile() != 0 || WriteLogFile() != 0)
	{
		printf("A fajlok mentese nem sikerult!\n");
	}
}

// Alarm handling
void CatchAlarm(int sig)
{
	if(rentedBikesCount > 0)
	{
		ReturnBike();
		alarm(5);
	}
}

int main(void)
{
	if(OpenBikeFile() == -1 || OpenLogFile() == -1)
	{
		return EXIT_FAILURE;
	}
	
    if(
		pipe(pipes[0]) < 0 ||
		pipe(pipes[1]) < 0 ||
		pipe(pipes[2]) < 0 ||
		pipe(pipes[3]) < 0 
	)
    {
        printf("[ERROR] create pipes\n");
        return -1;
    }

    printf("[PARENT] Create child1\n");
    if((pids[0] = fork()) < 0)
    {
        printf("[ERROR] Fork error\n");
        return -1;
    }

    if(pids[0] == 0)
    {
        // Child1 code
        char *message;
        
        close(pipes[0][0]);
        close(pipes[1][1]);
        
        do
        {
			message = ReadFromPipe(pipes[1][0]);
            printf("[ALLOMAS1] pipe message from parent: %s\n", message);
			
            if(atoi(message) != -1)
            {
				char *bikeIdStr;
				bikeIdStr = (char*) calloc(1, sizeof(char));
				
				strncpy(bikeIdStr, message + 1, strlen(message));
				
				switch(message[0])
				{
					case '0':
						printf("[ALLOMAS1] A %s szamu biciklit kikolcsonoztek!\n", bikeIdStr);
						WriteToPipe(pipes[0][1], "-1");
						free(bikeIdStr);
						break;
					case '1':
						srand((unsigned) time(NULL));
						int status = (rand() % 2);
						switch (status)
						{
							case 0:
								printf("[ALLOMAS1] A %s szamu biciklit Rossz allapotban hoztak vissza!\n", bikeIdStr);
								WriteToPipe(pipes[0][1], "0");
								free(bikeIdStr);
								break;
							case 1:
								printf("[ALLOMAS1] A %s szamu biciklit Jo allapotban hoztak vissza!\n", bikeIdStr);
								WriteToPipe(pipes[0][1], "1");
								free(bikeIdStr);
								break;
						}
						break;
				}
			}
		}
		while(atoi(message) != -1);
    }
    else if(pids[0] > 0)
    {
        // Parent code
        printf("[PARENT] Create child2\n");
		if((pids[1] = fork()) < 0)
		{
			printf("[ERROR] Fork error\n");
			return -1;
		}

		if(pids[1] == 0)
		{
			// Child2 code
			char *message;
			
			close(pipes[2][0]);
			close(pipes[3][1]);
			
			do
			{
				message = ReadFromPipe(pipes[3][0]);
				printf("[ALLOMAS2] pipe message from parent: %s\n", message);
				
				if(atoi(message) != -1)
				{
					char *bikeIdStr;
					bikeIdStr = (char*) calloc(1, sizeof(char));
					
					strncpy(bikeIdStr, message + 1, strlen(message));
					
					switch(message[0])
					{
						case '0':
							printf("[ALLOMAS2] A %s szamu biciklit kikolcsonoztek!\n", bikeIdStr);
							WriteToPipe(pipes[2][1], "-1");
							free(bikeIdStr);
							break;
						case '1':
							srand((unsigned) time(NULL));
							int status = (rand() % 2);
							switch (status)
							{
								case 0:
									printf("[ALLOMAS2] A %s szamu biciklit Rossz allapotban hoztak vissza!\n", bikeIdStr);
									WriteToPipe(pipes[2][1], "0");
									free(bikeIdStr);
									break;
								case 1:
									printf("[ALLOMAS2] A %s szamu biciklit Jo allapotban hoztak vissza!\n", bikeIdStr);
									WriteToPipe(pipes[2][1], "1");
									free(bikeIdStr);
									break;
							}
							break;
					}
				}
			}
			while(atoi(message) != -1);
		}
        else if(pids[1] > 0)
        {
			//Parent Main code
			struct sigaction alarmAction;
			alarmAction.sa_handler = CatchAlarm;
			sigemptyset(&alarmAction.sa_mask);
			alarmAction.sa_flags = SA_RESTART;
			sigaction(SIGALRM, &alarmAction, NULL);
			
			//signal (SIGALRM, CatchAlarm);
			
			// Close child1 related pipe endings
			close(pipes[0][1]);
			close(pipes[1][0]);
			
			// Close child2 related pipe endings
			close(pipes[2][1]);
			close(pipes[3][0]);
			
			// Start Menu
			printf("Rent an OP-BIKE! It's for FREE! projekt nyilvantartasa.\n");
			int end;
			do
			{
				do
				{
					printf("\n\n---------------\nMenu\n\n");
					printf("1. Biciklik Listazasa\n");
					printf("2. Egy Allomas Osszes Biciklije\n");
					printf("3. Uj Bicikli Hozzaadasa\n");
					printf("4. Bicikli Torlese\n");
					printf("5. Kolcsonzes\n");
					printf("6. Visszavetel\n");
					printf("7. Osszes Esemeny Listazasa\n");
					printf("8. Kilepes");
					printf("\n\n---------------\n");

					scanf("%d", &end);

					int c = getc(stdin);
					//printf("HERE: %d\n", c);
					
					if(end < 1 || end > 8)
					{
						printf("Hibas menupont, kerem probalja ujra!!!\n\n");
					}
				}
				while(end < 1 || end > 8);
			
				
				switch (end)
				{
					case 1:
						ListAllBike();
						break;
					case 2:
						ListAllBikeOnStation();
						break;
					case 3:
						CreateBike();
						break;
					case 4:
						RemoveBike();
						break;
					case 5:
						RentBike();
						break;
					case 6:
						while(rentedBikesCount > 0)
						{
							sleep(5);
							ReturnBike();
							rentedBikesCount--;
						}
						break;
					case 7:
						ListAllEvent();
						break;
					case 8:
						break;
				}
			}
			while (end != 8);
			
			WriteToPipe(pipes[1][1], "-1");
			WriteToPipe(pipes[3][1], "-1");
			close(pipes[0][0]);
			close(pipes[1][1]);
			close(pipes[2][0]);
			close(pipes[3][1]);
			close(pipes[4][0]);
			close(pipes[5][1]);

			SaveFiles();
			return EXIT_SUCCESS;
		}
	}
    
    return EXIT_FAILURE;
}
