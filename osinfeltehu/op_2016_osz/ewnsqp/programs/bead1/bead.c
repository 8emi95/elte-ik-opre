#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

typedef struct Event
{
	int id;
	char name[30];
} Event;

typedef struct Candidate
{
	char name[30];
	char email[40];
	int eventId;
	int position;
	char time[10];
} Candidate;

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
		printf("%d. rendezveny\nAzonosito: %d\nNev: %s\n",
			i + 1,
			events[i].id,
			events[i].name);
	}
}

void readDataFromFile(FILE* file, Event** pofevents, size_t* eventsSize, Candidate** pofcandidates, size_t* candidatesSize)
{
	fscanf(file, "%d", eventsSize);
	Event* events = *pofevents = (Event*)malloc(sizeof(Event) * (int)(*eventsSize));

	int i = 0;
	while(i < (int)*eventsSize)
	{
		fscanf(file, "%d %s", &events[i].id, events[i].name);
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
		fprintf(file, "%d %s\n", events[i].id, events[i].name);
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
	printf("Kerem adja meg rezdezveny azonositojat es nevet a kovetkezo formaban: azonosito(szam) nev!\n");
	
	Event newEvent;
	scanf("%d %s", &newEvent.id, newEvent.name);

	events = *pofevents = (Event*)realloc(*pofevents, sizeof(Event) * (int)(++*eventSize));
	events[(int)*eventSize - 1] = newEvent;

	strcpy(events[(int)*eventSize - 1].name, newEvent.name);
	
	printf("Sikeres rendezveny mentes, %d azonositoval es %s nevvel!\n",
		events[(int)*eventSize - 1].id,
		events[(int)*eventSize - 1].name);
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

void deleteEvent(Event** pofevents, size_t* eventSize)
{
	Event* events = *pofevents;
	
	int id;
	printf("Kerem adja meg a torolni kivant rendezveny azonositojat: ");
	scanf("%d", &id);
	int i, j;
	for (i = 0; i < (int)*eventSize; ++i)
	{
		if (events[i].id == id)
		{
			for (j = i; j < (int)*eventSize; j++)
			{
				events[j].id = events[j+1].id;
				strcpy(events[j].name, events[j+1].name);
			}
			--*eventSize;
		}
	}
	events = *pofevents = (Event*)realloc(*pofevents, sizeof(Event) * (int)(*eventSize));
	printf("Sikeres torles!\n");
}

int main(int argc, char** argv)
{
	size_t eventSize, candidatesSize;
	Event* events;
	Candidate* candidates;

	if (argc != 2)
	{
		printf("Hibas input!\n");
        return 1;
	}

	int menu;
	FILE* dataFile = fopen(argv[1], "r");
	readDataFromFile(dataFile, &events, &eventSize, &candidates, &candidatesSize);
	
	while (menu != 8)
	{
		printf("Kerem valasszon az alabbi menupontok kozul:\n 1 - Vendeg jelentkezes\n 2 - Vendegek listazasa\n 3 - Vendeg adatainak modositasa\n 4 - Vendeg adatainak torlese\n 5 - Rendezveny inditasa\n 6 - Rendezveny adatainak torlese\n 7 - Rendezvenyek listazasa\n 8 - Kilepes\n");
	    printf("\n");
		printf("Az opcio: ");

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
				deleteEvent(&events, &eventSize);
				break;
			case 7:
				printEvents(events, eventSize);
				break;
			case 8:
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