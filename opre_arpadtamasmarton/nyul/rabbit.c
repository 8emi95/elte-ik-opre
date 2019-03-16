#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "getdate.h"

///////////////////////////
// Execute command in geany
// "./%e" 4
///////////////////////////


// System tools
typedef enum {FALSE, TRUE} bool;
// Pipe handling
char pipeMessage[100];

char* IntToStr(int number)
{
	char numberStrArray[20];
	sprintf(numberStrArray, "%d", number);
	char *numberStrPointer = (char *) malloc(1 + strlen(numberStrArray));
	strncpy(numberStrPointer, numberStrArray, (int)strlen(numberStrArray));
	return numberStrPointer;
}

char* ConcatenateStrings(char *a, char *b)
{
	char *result = (char *) malloc(1 + strlen(a)+ strlen(b));
	strcpy(result, a);
    strcat(result, b);
	return result;
}

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














const char *cities[8] = {"BUDAPEST", "DEBRECEN", "MISKOLC", "SOPRON", "GYOR", "SZEGED", "ETYEK", "PECS"};

struct s_month
{
	int id;
	char *yearMonth;
	struct s_month *next;
};
typedef struct s_month month;

month *firstMonth = NULL;
month *newMonth = NULL;
month *actualMonth = NULL;
month *lastMonth = NULL;
int countMonths = 0;

struct s_rabbit
{
	int id;
	char *name;
	char *location;
	int young;
	int adult;
	int old;
	char *dateTime;
	char *full;
	struct s_rabbit *next;
};
typedef struct s_rabbit rabbit;

rabbit *firstRabbit = NULL;
rabbit *newRabbit = NULL;
rabbit *actualRabbit = NULL;
rabbit *lastRabbit = NULL;
int countRabbits = 0;

int IntComparator(const void *aa, const void *bb)
{
    const int *a = aa, *b = bb;
    return (*a < *b) ? -1 : (*a > *b);
}

int GetFirstFreeId()
{
	int freeId = 0;
	if(firstRabbit != NULL)
	{
		if(countRabbits != 1)
		{
			int rabbitIds[countRabbits], i = 0;
			actualRabbit = firstRabbit;
			while(actualRabbit != NULL)
			{
				rabbitIds[i] = actualRabbit->id;
				i++;
				actualRabbit = actualRabbit->next;
			}
			// printf("\n COUNTRABBITS: %d\n", countRabbits);
			qsort(rabbitIds, countRabbits, sizeof(int), IntComparator);
			
			i = 0;
			while(freeId == 0)
			{
				if((i < countRabbits - 1) && ((rabbitIds[i + 1] - rabbitIds[i]) > 1))
				{
					freeId = rabbitIds[i] + 1;
				}
				
				if(i == countRabbits - 1)
				{
					freeId = rabbitIds[i] + 1;
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
	
	return freeId;
}

int addRabbitToList(char *full)
{
	int i = 0;
	char *result = strtok(full, ";");
	rabbit *this;
	this = malloc(sizeof(rabbit));
	this->full = full;
	this->id = GetFirstFreeId();
	
	for(i = 0; result != NULL; i++)
	{
		switch (i)
		{
			case 0:
				this->name = (char*) malloc(strlen(result) + 1);
				strncat(this->name, result, strlen(result));
				break;
			case 1:
				this->location = (char*) malloc(strlen(result) + 1);
				strncat(this->location, result, strlen(result));
				break;
			case 2:
				this->young = atoi(result);
				break;
			case 3:
				this->adult = atoi(result);
				break;
			case 4:
				this->old = atoi(result);
				break;
			case 5:
				this->dateTime = (char*) malloc(strlen(result) + 1);
				strncat(this->dateTime, result, strlen(result));
				break;
		}
		result = strtok(NULL, ";");
	}
	
	this->next = NULL;
	
	if(firstRabbit == NULL)
	{
		lastRabbit = actualRabbit = firstRabbit = this;
		countRabbits++;
	}
	else
	{
		lastRabbit->next = this;
		lastRabbit = actualRabbit = this;
		countRabbits++;
	}
	
	return 0;
}

void listAllRabbit()
{
	// printf("Count: %d\n", countRabbits);
	if(firstRabbit != NULL)
	{
		actualRabbit = firstRabbit;
		while(actualRabbit != NULL)
		{
			printf("++++++++++++++++++++++++++++++++++++++++++++\n");
			printf("ID: %d\n", actualRabbit->id);
			printf("Megfigyelo: %s\n", actualRabbit->name);
			printf("Helyszin: %s\n", actualRabbit->location);
			printf("Fiatalok szama: %d\n", actualRabbit->young);
			printf("Felnottek szama: %d\n", actualRabbit->adult);
			printf("Idosek szama: %d\n", actualRabbit->old);
			printf("Megfigyeles idopontja: %s\n", actualRabbit->dateTime);
			printf("++++++++++++++++++++++++++++++++++++++++++++\n\n");
			
			actualRabbit = actualRabbit->next;
		}
	}
	else
	{
		printf("A megfigyelesek listaja ures!\n");
	}
}

int RemoveRabbit()
{
	if(firstRabbit != NULL)
	{
		char *rabbitIdStr;
		rabbitIdStr = (char*) calloc(1, sizeof(char));
		printf("\nAdd meg a torlendo megfigyeles szamat (0 - Megse):\n");
		scanf("%[^\n]s", rabbitIdStr);
		getchar();
		int rabbitId = atoi(rabbitIdStr);
		
		if(rabbitId)
		{
			rabbit *previousRabbit = NULL;
			actualRabbit = firstRabbit;
			previousRabbit = firstRabbit;
			
			if(firstRabbit->id == rabbitId)
			{
				firstRabbit = firstRabbit->next;
			}
			else
			{
				while(actualRabbit->id != rabbitId)
				{
					if(actualRabbit->next != NULL)
					{
						previousRabbit = actualRabbit;
						actualRabbit = actualRabbit->next;
					}
					else
					{
						printf("Nincs ilyen megfigyeles!\n\n");
						return -1;
					}
				}
			}
			
			previousRabbit->next = actualRabbit->next;
			free(actualRabbit);
		}
	}
	return 0;
}

int chooseLocation()
{
	char *choiceStr;
	choiceStr = (char*) calloc(1, sizeof(char));
	printf("\nAdd meg az uj telepules szamat\n(BUDAPEST - 1; DEBRECEN - 2; MISKOLC - 3; SOPRON - 4; GYOR - 5; SZEGED - 6; ETYEK - 7;PECS - 8;):\n");
	scanf("%[^\n]s", choiceStr);
	getchar();
	int choice = atoi(choiceStr);
	
	return choice - 1;
}

int getRabbitCount()
{
	char *choiceStr;
	choiceStr = (char*) calloc(1, sizeof(char));
	printf("\nAdd meg az uj szamot:\n");
	scanf("%[^\n]s", choiceStr);
	getchar();
	int choice = atoi(choiceStr);
	
	return choice;
}

void modifyRabbit()
{
	if(firstRabbit != NULL)
	{
		char *rabbitIdStr;
		rabbitIdStr = (char*) calloc(1, sizeof(char));
		printf("\nAdd meg a modositando megigyeles szamat (0 - Megse):\n");
		scanf("%[^\n]s", rabbitIdStr);
		getchar();
		int rabbitId = atoi(rabbitIdStr);
		
		if(rabbitId)
		{
			actualRabbit = firstRabbit;
			while(actualRabbit->id != rabbitId)
			{
				if(actualRabbit->next != NULL)
				{
					actualRabbit = actualRabbit->next;
				}
				else
				{
					printf("Nincs ilyen megfigyeles!\n\n");
				}
			}
			
			char *choiceStr;
			int choice = 0;
			
			do
			{
				choiceStr = (char*) calloc(1, sizeof(char));
				printf("\nAdd meg a modosithato adat szamat\n(Helyszin - 1; Fiatalok - 2; Felnottek - 3; Idosek - 4; Exit - 0;):\n");
				scanf("%[^\n]s", choiceStr);
				getchar();
				choice = atoi(choiceStr);
				int ind = 0;
				
				switch(choice)
				{
					case 1:
						ind = chooseLocation();
						actualRabbit->location = (char*) malloc(strlen(cities[ind]) + 1);
						strncat(actualRabbit->location, cities[ind], strlen(cities[ind]));
						break;
					case 2:
						actualRabbit->young = getRabbitCount();
						break;
					case 3:
						actualRabbit->adult = getRabbitCount();
						break;
					case 4:
						actualRabbit->old = getRabbitCount();
						break;
					default:
						break;
				}
			}
			while (choice);
		}
	}
}

int checkMonth(char *this)
{
	month *thisActual = firstMonth;
	
	if(thisActual != NULL)
	{
		while(thisActual != NULL)
		{
			if(!strncmp(this, thisActual->yearMonth, 7))
			{
				return 0;
			}
		}
	}
	else
	{
		return 1;
	}
	
	return 1;
}

void addMonths()
{
	int i = 1;
	month *this;
	this = malloc(sizeof(month));
	char tempYearMonth[8];
	
	if(firstRabbit != NULL)
	{
		actualRabbit = firstRabbit;
		while(actualRabbit != NULL)
		{
			strncpy(tempYearMonth, actualRabbit->dateTime, 7);
			if(checkMonth(tempYearMonth))
			{
				this->id = i;
				this->yearMonth = (char*) malloc(strlen(tempYearMonth));
				strncat(this->yearMonth, tempYearMonth, strlen(tempYearMonth));
				this->next = NULL;
				
				if(firstMonth == NULL)
				{
					lastMonth = actualMonth = firstMonth = this;
					countMonths++;
				}
				else
				{
					lastMonth->next = this;
					lastMonth = actualMonth = this;
					countMonths++;
				}
				
				// printf("%d - %s\n", this->id, this->yearMonth);
				i++;
			}
			
			actualRabbit = actualRabbit->next;
		}
	}
}

void listAllMonths()
{
	if(firstMonth != NULL)
	{
		actualMonth = firstMonth;
		printf("A kovetkezo honapok tartalmaznak adatot:\n");
		
		while(actualMonth != NULL)
		{
			printf("%d - %s\n", actualMonth->id, actualMonth->yearMonth);
			actualMonth = actualMonth->next;
		}
	}
	else
	{
		printf("A honapok listaja ures!\n");
	}
}

void showMonthData()
{
	if(firstRabbit != NULL)
	{
		addMonths();
		listAllMonths();
		char *choiceStr;
		choiceStr = (char*) calloc(1, sizeof(char));
		printf("\nAdd meg a honap szamat (0 - Megse):\n");
		scanf("%[^\n]s", choiceStr);
		getchar();
		int choice = atoi(choiceStr);
		
		if(choice)
		{
			actualMonth = firstMonth;
			while(actualMonth->id != choice)
			{
				actualMonth = actualMonth->next;
			}
			
			int young = 0, adult = 0, old = 0;
			
			char *tempYearMonth = actualMonth->yearMonth;
			printf("HERE: %s\n", actualMonth->yearMonth);
			
			
			actualRabbit = firstRabbit;
			while(actualRabbit != NULL)
			{
				if(!strncmp(tempYearMonth, actualRabbit->dateTime, 7))
				{
					young += actualRabbit->young;
					adult += actualRabbit->adult;
					old += actualRabbit->old;
				}
				actualRabbit = actualRabbit->next;
			}
			
			printf("A %s honap adatai:\n\n", tempYearMonth);
			printf("Fiatal: %d\n", young);
			printf("Felnott: %d\n", adult);
			printf("Idos: %d\n", old);
		}
	}
}

int main(int argc, char *argv[])
{
	int i;
	int status;
	int	jobs = atoi(argv[1]);
	int	pipeArray[(jobs * 2)][2];
	bool isDead = FALSE;
	int jobsStatus[jobs];
	
	pid_t pids[jobs];
	
	srand((unsigned) time(NULL));
	
	for(i = 0; i < jobs; i++)
	{
		if(pipe(pipeArray[i * 2]) < 0 || pipe(pipeArray[i * 2 + 1]) < 0)
		{
			perror("Error on creating pipe");
			return -1;
		}
		
		pids[i] = fork();
		switch(pids[i])
		{
			case -1:
				perror("Error on Fork");
				return -1;
				break;
			case 0:
				printf("in CHILD%d\n", i + 1);
				char *message;
        
				close(pipeArray[i * 2][0]);
				close(pipeArray[i * 2 + 1][1]);
				
				do
				{
					// printf("i: %d\n", i);
					message = ReadFromPipe(pipeArray[i * 2 + 1][0]);
					// printf("message: %s", message);
					// printf("\n[CHILD%d] pipe message from parent: %s\n\n", i, message);
					if(strncmp(message, "[MEGFIGYELO", 11) == 0)
					{
						printf("\n\nA [MEGFIGYELO%d] expedicio megfigyelesre kesz!\n", i + 1);
						WriteToPipe(pipeArray[i * 2][1], "continue");
					}
					else if(strncmp(message, "START", 5) == 0)
					{
						WriteToPipe(pipeArray[i * 2][1], "arrived");
					}
					else if(strncmp(message, "OBSERVE", 7) == 0)
					{
						rabbit *this;
						this = malloc(sizeof(rabbit));
						
						this->name = ConcatenateStrings("MEGFIGYELO", IntToStr(i + 1));
						srand((unsigned) time(NULL));
						int ind = rand() % 8;
						this->location = cities[ind];
						srand((unsigned) time(NULL));
						this->young = rand() % 10;
						srand((unsigned) time(NULL));
						this->adult = rand() % 10;
						srand((unsigned) time(NULL));
						this->old = rand() % 10;
						
						char *temp = malloc(strlen(this->name) + strlen(this->location) + 30);
						
						temp = ConcatenateStrings(this->name, ";");
						temp = ConcatenateStrings(temp, this->location);
						temp = ConcatenateStrings(temp, ";");
						temp = ConcatenateStrings(temp, IntToStr(this->young));
						temp = ConcatenateStrings(temp, ";");
						temp = ConcatenateStrings(temp, IntToStr(this->adult));
						temp = ConcatenateStrings(temp, ";");
						temp = ConcatenateStrings(temp, IntToStr(this->old));
						temp = ConcatenateStrings(temp, ";");
						temp = ConcatenateStrings(temp, getDateTime());
						
						this->full = temp;
						srand((unsigned) time(NULL));
						sleep(((rand() % 4) + 2));
						
						WriteToPipe(pipeArray[i * 2][1], this->full);
					}
					else if(atoi(message) == -1)
					{
						WriteToPipe(pipeArray[i * 2][1],"Exiting!\n\n");
					}
				}
				while(atoi(message) != -1);
				
				return 0;
				break;
			default:
				// Parent process
				// printf("PARENT\n");
				close(pipeArray[i * 2][1]);
				close(pipeArray[i * 2 + 1][0]);

		}
	}
	
	char *parentMessage;
	int arrivedChildren = 0;
	
	for(i = 0; i < jobs; i++)
	{
		WriteToPipe(pipeArray[i * 2 + 1][1], "START");
		parentMessage = ReadFromPipe(pipeArray[i * 2][0]);
		if(strncmp(parentMessage, "arrived", 7) == 0)
		{
			arrivedChildren++;
			jobsStatus[i] = 1;
		}
	}

	if(arrivedChildren == jobs)
	{
		printf("[PARENT]Every children are arrived\n");
	}
	else
	{
		printf("WTF????????");
	}
	
	for(i = 0; i < jobs; i++)
	{
		srand((unsigned) time(NULL));
		if((rand() % 2))
		{
			printf("%d szamu faradt expedicio visszahivasa!\n", i + 1);
			kill(pids[i], SIGTERM);
			if (waitpid(pids[i], &status, WNOHANG) == pids[i])
			{
				isDead = TRUE;
			}
			if(!isDead)
			{
				kill(pids[i], SIGKILL);
			}
			
			close(pipeArray[i * 2][0]);
			close(pipeArray[i * 2 + 1][1]);
			
			jobsStatus[i] = 0;
		}
	}
	
	for(i = 0; i < jobs; i++)
	{
		if(jobsStatus[i])
		{
			char *a = ConcatenateStrings("[MEGFIGYELO", IntToStr(i + 1));
			a = ConcatenateStrings(a, "]");
			WriteToPipe(pipeArray[i * 2 + 1][1], a);
			parentMessage = ReadFromPipe(pipeArray[i * 2][0]);
			// printf("\n[PARENT] Message from [CHILD%d]: %s\n", i, parentMessage);
			
			WriteToPipe(pipeArray[i * 2 + 1][1], "OBSERVE");
			parentMessage = ReadFromPipe(pipeArray[i * 2][0]);
			addRabbitToList(parentMessage);
			// printf("[PARENT] Message from [CHILD%d]: %s\n", i + 1, parentMessage);
			
			WriteToPipe(pipeArray[i * 2 + 1][1], "-1");
			parentMessage = ReadFromPipe(pipeArray[i * 2][0]);
			// printf("\n[PARENT] Message from [CHILD%d]: %s\n", i, parentMessage);
			close(pipeArray[i * 2][0]);
			close(pipeArray[i * 2 + 1][1]);
		}
	}
	
	listAllRabbit();
	showMonthData();
	listAllRabbit();
	RemoveRabbit();
	listAllRabbit();
	modifyRabbit();
	listAllRabbit();
	
	return 0;
}
