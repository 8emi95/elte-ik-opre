#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h> 
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>

typedef struct Event
{
	int id;
	char name[30];
	char time[10];
	char location[30];
} Event;

typedef struct Candidate
{
	char name[30];
	char email[40];
	int eventId;
	int position;
	char time[10];
} Candidate;

void writestd(const char* str);
void printCandidates(Candidate* candidates, size_t candidatesSize);
void printEvents(Event* events, size_t eventSize);
void readDataFromFile(FILE* file, Event** pofevents, size_t* eventsSize, Candidate** pofcandidates, size_t* candidatesSize);
void writeDataToFile(FILE* file, Event* events, size_t eventsSize, Candidate* candidates, size_t candidatesSize);
void signCandidate(Candidate** pofcandidates, size_t* candidatesSize);
void createEvent(Event** pofevents, size_t* eventSize);
void modifyCandidate(Candidate* candidates);
void deleteCandidate(Candidate** pofcandidates, size_t* candidatesSize);
void deleteEvent(Event** pofevents, size_t* eventSize, int id);
void prepareToDeleteEvent(Event** pofevents, size_t* eventSize);
size_t getCandidatesFromEvent(Event* event, Candidate* candidates, size_t candidatesSize, Candidate*** result);
void partnersignal_handler(int signumber);
Event* findEventByLocalTime(Event* events, size_t size);
void printOptions();

int fd[2];
size_t eventSize, candidatesSize;
Event* events;
Candidate* candidates;

Event* eventToDo;

void writestd(const char* str)
{
	write(STDOUT_FILENO, str, strlen(str));
}

void printCandidates(Candidate* candidates, size_t candidatesSize)
{
	int i;
	for (i = 0; i < (int)candidatesSize; ++i)
	{
		printf("%d. vendeg\nNev: %s\nEmail: %s\nRendezveny azonosito: %d\nFeliratkozas sorszama: %d\nFeliratkozas ideje: %s\n",
			i + 1,
			candidates[i].name,
			candidates[i].email,
			candidates[i].eventId,
			candidates[i].position,
			candidates[i].time);
	}
}

void printEvents(Event* events, size_t eventSize)
{
	int i;
	
	for (i = 0; i < (int)eventSize; ++i)
	{
		printf("%d. rendezveny\n\
Azonosito: %d\n\
Nev: %s\n\
Ido: %s\n\
Helyszin: %s\n",
			i + 1,
			events[i].id,
			events[i].name,
			events[i].time,
			events[i].location);
	}
}

void readDataFromFile(FILE* file, Event** pofevents, size_t* eventsSize, Candidate** pofcandidates, size_t* candidatesSize)
{
	fscanf(file, "%d", eventsSize);
	Event* events = *pofevents = (Event*)malloc(sizeof(Event) * (int)(*eventsSize));

	int i = 0;
	while(i < (int)*eventsSize)
	{
		fscanf(file, "%d %s %s %s", &events[i].id, events[i].name, events[i].time, events[i].location);
		++i;
	}

	fscanf(file, "%d", candidatesSize);
	Candidate* candidates = *pofcandidates = (Candidate*)malloc(sizeof(Candidate) * (int)(*candidatesSize));

	i = 0;
	while(i < (int)*candidatesSize)
	{  
		fscanf(file, "%s %s %d %d %s", candidates[i].name, candidates[i].email, &candidates[i].eventId, &candidates[i].position, candidates[i].time);
		++i;
	}
}

void writeDataToFile(FILE* file, Event* events, size_t eventsSize, Candidate* candidates, size_t candidatesSize)
{
	fprintf(file, "%d\n", eventsSize);
	
	int i = 0;

	while(i < (int)eventsSize)
	{
		fprintf(file, "%d %s %s %s\n", events[i].id, events[i].name, events[i].time, events[i].location);
		++i;
	}

	fprintf(file, "%d\n", candidatesSize);

	i = 0;
	while(i < (int)candidatesSize)
	{
		fprintf(file, "%s %s %d %d %s\n", candidates[i].name, candidates[i].email, candidates[i].eventId, candidates[i].position, candidates[i].time);
		++i;
	}
}

void signCandidate(Candidate** pofcandidates, size_t* candidatesSize)
{
	Candidate* candidates = *pofcandidates;
	printf("Kerem adja meg a nevet, email cimet, rendezveny azonositojat a kovetkezo formaban: nev email rendezvenyazon!\n");
	
	Candidate newCandidate;
	scanf("%s %s %d", newCandidate.name, newCandidate.email, &newCandidate.eventId);
	newCandidate.position = *candidatesSize = *candidatesSize + 1;

	char tmp[20];
	time_t now;
	struct tm *now_tm;
	char buffer[30];
	now = time(NULL);
	now_tm = localtime(&now);
	sprintf(tmp, "%d", now_tm->tm_hour);
	strcpy(buffer, tmp);
	strcat(buffer, ":");
	strcat(tmp, ":");
	sprintf(tmp, "%d", now_tm->tm_min);
	strcat(buffer, tmp);

	candidates = *pofcandidates = (Candidate*)realloc(*pofcandidates, sizeof(Candidate) * (int)(*candidatesSize));
	candidates[(int)*candidatesSize - 1] = newCandidate;

	strcpy(candidates[(int)*candidatesSize - 1].name, newCandidate.name);
	strcpy(candidates[(int)*candidatesSize - 1].email, newCandidate.email);
	strcpy(candidates[(int)*candidatesSize - 1].time, buffer);

	printf("On a %d. jelentkezo\n", *candidatesSize);
}

void createEvent(Event** pofevents, size_t* eventSize)
{
	Event* events = *pofevents;
	printf("Kerem adja meg rezdezveny azonositojat es nevet a kovetkezo formaban: azonosito(szam) nev ido(hh:mm) helyszin!\n");
	
	Event newEvent;
	scanf("%d %s %s %s", &newEvent.id, newEvent.name, newEvent.time, newEvent.location);

	events = *pofevents = (Event*)realloc(*pofevents, sizeof(Event) * (int)(++*eventSize));
	events[(int)*eventSize - 1] = newEvent;

	strcpy(events[(int)*eventSize - 1].name, newEvent.name);
	strcpy(events[(int)*eventSize - 1].time, newEvent.time);
	strcpy(events[(int)*eventSize - 1].location, newEvent.location);
	
	printf("Sikeres rendezveny mentes, %d azonositoval, %s idovel, %s nevvel es %s helyszinnel!\n",
		events[(int)*eventSize - 1].id,
		events[(int)*eventSize - 1].time,
		events[(int)*eventSize - 1].name,
		events[(int)*eventSize - 1].location);
}

void modifyCandidate(Candidate* candidates)
{
	int candidateNum;
	char tmpName[20];
	char tmpEmail[30];
	int tmpId;
	
	printf("Kerem adja meg a modositani kivant jelentkezo sorszamat: ");
	scanf("%d", &candidateNum);

	printf("Kerem adja meg a nevet, email cimet, rendezveny azonositojat a kovetkezo formaban: nev email rendezvenyazon!\n");
	scanf("%s %s %d", tmpName, tmpEmail, &candidates[candidateNum - 1].eventId);
	strcpy(candidates[candidateNum - 1].name, tmpName);
	strcpy(candidates[candidateNum - 1].email, tmpEmail);
	
	printf("Sikeres modositas!\n");
}

void deleteCandidate(Candidate** pofcandidates, size_t* candidatesSize)
{
	Candidate* candidates = *pofcandidates;
	char tmpName[20];
	printf("Kerem adja meg a torolni kivant jelentkezo nevet: ");
	scanf("%s", tmpName);
	int i, j;
	for (i = 0; i < (int)*candidatesSize; ++i)
	{
		if (strcmp(candidates[i].name, tmpName) == 0)
		{
			for (j = i; j < (int)*candidatesSize; j++)
			{
				strcpy(candidates[j].name, candidates[j+1].name);
				strcpy(candidates[j].email, candidates[j+1].email);
				candidates[j].eventId = candidates[j+1].eventId;
				candidates[j].position = candidates[j+1].position;
				strcpy(candidates[j].time, candidates[j+1].time);
			}
			--*candidatesSize;
		}
	}
	candidates = *pofcandidates = (Candidate*)realloc(*pofcandidates, sizeof(Candidate) * (int)(*candidatesSize));
	printf("Sikeres torles!\n");
}

void deleteEvent(Event** pofevents, size_t* eventSize, int id)
{
	Event* events = *pofevents;
	int i, j;
	for (i = 0; i < (int)*eventSize; ++i)
	{
		if (events[i].id == id)
		{
			for (j = i; j < (int)*eventSize; j++)
			{
				events[j].id = events[j+1].id;
				strcpy(events[j].name, events[j+1].name);
				strcpy(events[j].time, events[j+1].time);
				strcpy(events[j].location, events[j+1].location);
			}
			--*eventSize;
		}
	}
	events = *pofevents = (Event*)realloc(*pofevents, sizeof(Event) * (int)(*eventSize));
}

void prepareToDeleteEvent(Event** pofevents, size_t* eventSize)
{
	int id;
	printf("Kerem adja meg a torolni kivant rendezveny azonositojat: ");
	scanf("%d", &id);
	deleteEvent(pofevents, eventSize, id);
	printf("Sikeres torles!\n");
}

size_t getCandidatesFromEvent(Event* event, Candidate* candidates, size_t candidatesSize, Candidate*** result)
{
	int i = 0;

	Candidate** tmpArray = malloc(sizeof(Candidate*) * (int)candidatesSize);
	size_t tmpArrayLength = 0;
	
	while(i < candidatesSize)
	{
		if (candidates[i].eventId == event->id)
		{
			tmpArray[tmpArrayLength] = candidates + i;
			++tmpArrayLength;
		}
		++i;
	}
	*result = tmpArray;

	return tmpArrayLength;
}

void partnersignal_handler(int signumber)
{
	char* answer1 = "Szervezo: a resztvevoket kuldom\n";
	
	Candidate** result = NULL;
	size_t resultLength = getCandidatesFromEvent(eventToDo, candidates, candidatesSize, &result);

	write(STDOUT_FILENO, answer1, strlen(answer1));
	
	if (resultLength == 0) 
	{
		free(result);
		write(fd[1], "\0", 1);
	}
	else
	{
		char names[1024];
		int size = 0;
		
		int i = 0;
		while (i < resultLength)
		{
			strcat(names, result[i]->name);
			strcat(names, "\n");
			size += strlen(result[i]->name) + 1;
			++i;
		}
		
		free(result);
		
		write(fd[1], &names, size);
	}
}

Event* findEventByLocalTime(Event* events, size_t size)
{
	if (events == NULL) return NULL;

	time_t rawtime;
	time(&rawtime);
	
	struct tm* current_time;
	current_time = localtime(&rawtime);
	
	char timestr[6];
	strftime(timestr, sizeof timestr, "%R", current_time);
	
	Event* firstEvent = NULL;
	
	int i = 0;
	while(firstEvent == NULL && i < size)
	{
		if (strcmp(events[i].time, timestr) == 0)
		{
			firstEvent = events + i;
		}
		++i;
	}
	
	return firstEvent;
}

void printOptions()
{
	printf("Kerem valasszon az alabbi menupontok kozul:\n\
1 - Vendeg jelentkezes\n\
2 - Vendegek listazasa\n\
3 - Vendeg adatainak modositasa\n\
4 - Vendeg adatainak torlese\n\
5 - Rendezveny inditasa\n\
6 - Rendezveny adatainak torlese\n\
7 - Rendezvenyek listazasa\n\
[8 - Rendezveny lebonyolitasa]\n\
9 - Kilepes\n\
Az opcio: ");
	fflush(stdout);
}

void doTheEvent(Event* event, size_t eventSize);
void doEventIfPossible(Event** pofevents, size_t* eventSize)
{
	int gotone = 0;
	do
	{
		eventToDo = findEventByLocalTime(*pofevents, *eventSize);
		if (eventToDo != NULL)
		{
			gotone = 1;
			printf("\n\nEgy esemeny lebonyolitasra kerul...\n");
			doTheEvent(eventToDo, *eventSize);
			deleteEvent(pofevents, eventSize, eventToDo->id);
		}
	} while(eventToDo != NULL);
	if (gotone) printOptions();
}

void doTheEvent(Event* event, size_t eventSize)
{
	signal(SIGUSR1, partnersignal_handler);
	pid_t partner = fork();
	
	if (partner > 0)
	{
		/* parent */
		writestd("Szervezo: kuldom a rendezveny helyszinet\n");
		write(fd[1], event->location, sizeof(event->location));
		pause();
		waitpid(partner, NULL, 0);
	}
	else
	{
		/* child */
		char* buffer = malloc(80);
		read(fd[0], buffer, 80);
		
		printf("Partner: itt vagyok a '%s' helyszinen, varom a resztvevoket!\n", buffer);
		kill(getppid(), SIGUSR1);
		
		char buffer2[1024];
		int nbyte = read(fd[0], &buffer2, sizeof(buffer2));
		
		char names[34][30];
		int char_index = 0;
		int names_length = 0;
		int i = 0;
		while (i < nbyte)
		{
			if (buffer2[i] == '\n')
			{
				++names_length;
				char_index = 0;
			}
			else
			{
				names[names_length][char_index] = buffer2[i];
				++char_index;
			}
			++i;
		}
		
		printf("Partner: a resztvevok szama: %d\n", names_length);
		printf("Partner: a rendezveny varhato resztvevoi:\n");
		
		i = 0;
		while(i < names_length)
		{
			printf("\t%d. resztvevo: %s\n", i + 1, names[i]);
			++i;
		}
		
		printf("Partner: a rendezveny lebonyolitasra kerul...\n");
		
		srand(time(NULL));
		printf("Partner: a rendezveny %d%%-ban volt sikeres\n", rand() % 100);
	
		int appearance[names_length];
		int mindenki = 1;
		i = 0;
		while(i < names_length)
		{
			appearance[i] = (rand() % 10) != 0;
			mindenki *= appearance[i];
			++i;
		}
		
		if (mindenki)
		{
			printf("Partner: mindenki megjelent a rendezvenyen\n");
		}
		else
		{
			printf("Partner: a kovetkezo resztvevok nem jelentek meg:\n");
			i = 0;
			while(i < names_length)
			{
				if (!appearance[i])
					printf("\t%s\n", names[i]);
				++i;
			}
		}
		printf("\n");
		exit(0);
	}
}

void sigalarm_handler(int signum)
{
	doEventIfPossible(&events, &eventSize);
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Hibas input!\n");
        return 1;
	}
	
	int menu;
	
	pipe(fd);
	FILE* dataFile = fopen(argv[1], "r");
	readDataFromFile(dataFile, &events, &eventSize, &candidates, &candidatesSize);
	eventToDo = NULL;
	
	signal(SIGALRM, sigalarm_handler);
	
	struct itimerval timer;
	timer.it_interval.tv_sec = 60;
	timer.it_interval.tv_usec = 0;
	timer.it_value.tv_sec = 60;
	timer.it_value.tv_usec = 0;       
	setitimer(ITIMER_REAL, &timer, NULL);
	
	while (menu != 9)
	{
		printOptions();

		scanf("%d", &menu);
		switch(menu)
		{
			case 1:
				signCandidate(&candidates, &candidatesSize);
				break;
			case 2:
				printCandidates(candidates, candidatesSize);
				break;
			case 3:
				printCandidates(candidates, candidatesSize);
				modifyCandidate(candidates);
				break;
			case 4:
				printCandidates(candidates, candidatesSize);
				deleteCandidate(&candidates, &candidatesSize);
				break;
			case 5:
				createEvent(&events, &eventSize);
				break;
			case 6:
				printEvents(events, eventSize);
				prepareToDeleteEvent(&events, &eventSize);
				break;
			case 7:
				printEvents(events, eventSize);
				break;
			case 8:
				doEventIfPossible(&events, &eventSize);
				break;
			case 9:
				freopen(argv[1], "w+", dataFile);
				writeDataToFile(dataFile, events, eventSize, candidates, candidatesSize);
 				fclose(dataFile);
				exit(0);
				break;
			default:
				printf("Nincs ilyen lehetoseg: %d!\n", menu);
				break;
		}
	}
	return 0;
}