#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

typedef enum {FALSE, TRUE} bool;
typedef enum {FOGLALT, SZABAD, ROSSZ} status_t;
typedef enum {KOLCSONZES, VISSZAHOZAS} action_t;

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
	}
	else
	{
		actualBike->next = newBike;
		lastBike = actualBike->next;
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

int RentBike(bool isReturn)
{
	char *bikeIdStr;
	bikeIdStr = (char*) calloc(1, sizeof(char));
	printf("\nAdd meg a ");
	if(isReturn == 1)
		printf("visszahozasra");
	else
		printf("berlesre");
	printf(" valasztott bicikli szamat:\n");
	scanf("%[^\n]s", bikeIdStr);
	getchar();
	int bikeId = atoi(bikeIdStr);
	
	actualBike = firstBike;
	while(actualBike->bikeId != bikeId)
	{
		if(actualBike->next != NULL)
		{
			actualBike = actualBike->next;
		}
		else
		{
			printf("Nincs ilyen bicikli!\n\n");
			return -1;
		}
	}
	
	if(isReturn == FALSE)
	{
		if(actualBike->bikeStatus == SZABAD)
		{
			char *userName;
			userName = (char*) calloc(1, sizeof(char));
			printf("\nKerem a felhasznalo nevet:\n");
			scanf("%[^\n]s", userName);
			getchar();
			
			AddEventToList(actualBike->bikeId, userName, KOLCSONZES, GetTime());
			
			actualBike->bikeStatus = FOGLALT;
			printf("A(z) %d szamu bicikli sikeresen ki lett berelve!\n\n", actualBike->bikeId);
		}
		else
		{
			printf("A bicikli nem berelheto!\n\n");
		}
	}
	else
	{
		if(actualBike->bikeStatus == FOGLALT)
		{
			char *userName;
			userName = (char*) calloc(1, sizeof(char));
			printf("\nKerem a felhasznalo nevet:\n");
			scanf("%[^\n]s", userName);
			getchar();
			
			AddEventToList(actualBike->bikeId, userName, VISSZAHOZAS, GetTime());
			
			actualBike->bikeStatus = SZABAD;
			printf("A(z) %d szamu biciklit visszahozta!\n\n", actualBike->bikeId);
		}
		else
		{
			printf("A bicikli nincs kiberelve!\n\n");
		}
	}
	
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
	char *bikeIdStr;
	bikeIdStr = (char*) calloc(1, sizeof(char));
	printf("\nAdd meg az uj bicikli szamat:\n");
	scanf("%[^\n]s", bikeIdStr);
	getchar();
	int bikeId = atoi(bikeIdStr);
	
	actualBike = firstBike;
	while(actualBike != NULL)
	{
		if(actualBike->bikeId == bikeId)
		{
			printf("HIBA: Letezik bicikli a megadott azonositoval!\n");
			return -1;
		}
		actualBike = actualBike->next;
	}
	
	char *stationIdStr;
	stationIdStr = (char*) calloc(1, sizeof(char));
	printf("\nAdd meg az uj bicikli allomasat:\n");
	scanf("%[^\n]s", stationIdStr);
	getchar();
	int stationId = atoi(stationIdStr);
	
	AddBikeToList(bikeId, stationId, SZABAD);
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
		close(dataFileReadDescriptor);
		return -1;
	}
}

int WriteBikeFile()
{
	unlink("./OP_BIKE");
	int logFileAppendDescriptor = creat("./OP_BIKE", 0644 | O_WRONLY | O_APPEND);
	
	actualBike = firstBike;
	while(actualBike != NULL)
	{
		char *bikeIdStr = IntToStr(actualBike->bikeId);
		char *stationIdStr = IntToStr(actualBike->stationId);
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
		
		char *newLine = (char *) malloc(4 + strlen(bikeIdStr) + strlen(stationIdStr) + strlen(status));
				
		newLine = ConcatenateStrings(bikeIdStr, ",");
		newLine = ConcatenateStrings(newLine, stationIdStr);
		newLine = ConcatenateStrings(newLine, ",");
		newLine = ConcatenateStrings(newLine, status);
		newLine = ConcatenateStrings(newLine, "\n");
		
		if(logFileAppendDescriptor != 0)
		{
			if (write(logFileAppendDescriptor, newLine, strlen(newLine)) != strlen(newLine)) {
				close(logFileAppendDescriptor);
				return -1;
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
		close(logFileReadDescriptor);
		return -1;
	}
}

int WriteLogFile()
{
	unlink("./OP_BIKE.RENT");
	int logFileAppendDescriptor = creat("./OP_BIKE.RENT", 0644 | O_WRONLY | O_APPEND);
	
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
			newLine = ConcatenateStrings(newLine, "\n");
		}
		
		if(logFileAppendDescriptor != 0)
		{
			if (write(logFileAppendDescriptor, newLine, strlen(newLine)) != strlen(newLine)) {
				close(logFileAppendDescriptor);
				return -1;
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

void mentes()
{
	if(WriteBikeFile() != 0 || WriteLogFile() != 0)
	{
		printf("A fajlok mentese nem sikerult!");
	}
}

int main(void)
{
	if(OpenBikeFile() == -1 || OpenLogFile() == -1)
	{
		return EXIT_FAILURE;
	}
	
	printf("Rent an OP-BIKE! It's for FREE! projekt nyilvantartasa.\n");

	int end;
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
		getchar();

		switch (end) {
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
				RentBike(FALSE);
				break;
			case 6:
				RentBike(TRUE);
				break;
			case 7:
				ListAllEvent();
				break;
			case 8:
				break;
			default:
				printf("\n\nNem letezo menupont!\n");
				break;
		}
	} while (end != 8);

	mentes();
	return EXIT_SUCCESS;
	/*OpenBikeFile();
	ListAllBike();
	AddBikeToList(21, 2, FOGLALT);
	//AddBikeToList(2, 1, SZABAD);
	//AddBikeToList(1, 3, ROSSZ);
	ListAllBike();
	//CreateBike();
	//ListAllBike();
	//RentBike(FALSE);
	//RentBike(TRUE);
	//ListAllBike();
	//ListAllBikeOnStation();
	//RemoveBike();
	//ListAllBike();
	//AddEventToList(11, "u2", VISSZAHOZAS, "Sun Nov  2 21:46:00 2014");
	//AddEventToList(1, "u1", VISSZAHOZAS, "Sun Nov  2 21:44:00 2014");
	//AddEventToList(8, "u3", KOLCSONZES, "Sun Nov  2 21:41:00 2014");
	//ListAllEvent();
	//OpenLogFile();
	//ListAllEvent();
	//AddEventToList(8, "u3", KOLCSONZES, "Sun Nov  2 21:41:00 2014");
	//getchar();
	//WriteLogFile();
	WriteBikeFile();*/
}
