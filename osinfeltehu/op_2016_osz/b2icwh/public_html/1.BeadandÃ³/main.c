#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <time.h>

struct Visitor
{
	char name[20];
	char eMailAddress[40];
	char eventID[5];
	char registrationTime[30];
};

struct Event
{
	char id[5];
	char name[30];
};

int numberOfEvents = 0;
int numberOfVisitors = 0;
struct Event events[1000];
struct Visitor visitors[1000];

int isFileEmpty(FILE * file)
{
    long savedOffset = ftell(file);
    fseek(file, 0, SEEK_END);

    if (ftell(file) == 0){
        return 1;
    }

    fseek(file, savedOffset, SEEK_SET);
    return 0;
}

void writeToFile()
{
	/*visitors.txt*/
	FILE * fileVisitor = fopen("visitors.txt", "w");
	if(fileVisitor == NULL)
	{
		printf("Fajl megnyitasi hiba, visitors.txt, iras!\n");
		exit(1);
	}
	int i;
	for(i = 0; i < numberOfVisitors; i++)
	{
		if(strcmp(visitors[i].name, "###") != 0)
		{
			fprintf(fileVisitor, "%s\t\t%s\t\t%s\t\t%s\n", visitors[i].name, visitors[i].eMailAddress, visitors[i].eventID, visitors[i].registrationTime);
		}
	}
	fclose(fileVisitor);

	/*events.txt-*/
	FILE * fileEvent = fopen("events.txt", "w");
	if(fileEvent == NULL)
	{
		printf("Fajl megnyitási hiba, events.txt, iras!\n");
		exit(1);
	}
	for(i = 0; i < numberOfEvents; i++)
	{
		if(strcmp(events[i].id, "###") != 0)
		{
			fprintf(fileEvent, "%s\t\t%s\n", events[i].id, events[i].name);
		}
	}
	fclose(fileEvent);
}

void readFromFiles()
{
	/*visitors.txt*/
	FILE * fileVisitor = fopen("visitors.txt", "r");
	if(fileVisitor == NULL)
	{
        printf("Fajl megnyitási hiba, events.txt, iras!\n");
		exit(1);
	}
	if(isFileEmpty(fileVisitor) == 0)
	{
		while(!feof(fileVisitor))
		{
			fscanf(fileVisitor, "%s\t\t%s\t\t%s\t\t%s\n", &visitors[numberOfVisitors].name, &visitors[numberOfVisitors].eMailAddress, &visitors[numberOfVisitors].eventID, &visitors[numberOfVisitors].registrationTime);
			numberOfVisitors++;
		}
	}
	fclose(fileVisitor);

	/*events.txt*/
	FILE * fileEvent = fopen("events.txt", "r");
	if(fileEvent == NULL)
	{
		printf("Fajl megnyitási hiba, events.txt, olvasas!\n");
		exit(1);
	}
	if(isFileEmpty(fileEvent) == 0)
	{
		while(!feof(fileEvent))
		{
			fscanf(fileEvent, "%s\t\t%s\n", &events[numberOfEvents].id, &events[numberOfEvents].name);
			numberOfEvents++;
		}
	}
	fclose(fileEvent);
}

void printMenu()
{
	printf("A KingOfStands elektronikus latogatoi nyilvantartasa :\n\n");
	printf("1 - Jelentkezes esemenyre\n");
	printf("2 - Jelentkezettek listazasa\n");
	printf("3 - Vendeg adatainak modositasa\n");
	printf("4 - Vendeg adatainak torlese\n");
	printf("5 - Uj esemeny felvetele\n");
	printf("6 - Esemeny torlese\n");
	printf("0 - Kilepes\n\n");
}

int iseventIDPresent(char id[])
{
	int i;
	for(i = 0; i < numberOfEvents; i++)
	{
		if(strcmp(id, events[i].id) == 0)
		{
			return 1;
		}
	}
	return 0;
}

void menuRegisterToEvent()
{
	system("clear");
	printf("Uj vendeg felvetele\n\n");
	printf("1 - Felvesz egy uj vendeget\n");
	printf("0 - Kilepes\n\n");
	int input;
	printf("Adjon meg egy szamot: ");
	scanf("%d", &input);
	if(input == 1)
	{
		printf("Aktiv esemenyek:\n");
		int i;
		for(i = 0; i < numberOfEvents; i++)
		{
			if(strcmp(events[i].id, "###") != 0)
			{
				printf("%d - Id: %s, Nev: %s\n", i+1, events[i].id, events[i].name);
			}
		}
		struct Visitor Visitor;
		int response = 1;
		do
		{
			printf("\nAdja meg a vendeg nevet: ");
			scanf("%s", Visitor.name);
			printf("Adja meg a vendeg e-mail cimet: ");
			scanf("%s", Visitor.eMailAddress);
			printf("Adja meg az esemeny azonositojat: ");
			scanf("%s", Visitor.eventID);
			time_t mytime = time(NULL);
			char time[40];
			strcpy(time, ctime(&mytime));
			time[strlen(time) - 1] = '\0';
			for(i = 0; i < strlen(time); i++)
			{
				if(time[i] == ' ')
				{
					time[i] = '_';
				}
			}
			strcpy(Visitor.registrationTime, time);
			response = iseventIDPresent(Visitor.eventID);
		}while(response == 0);

		visitors[numberOfVisitors] = Visitor;
		numberOfVisitors++;

		int place = 0;
		for(i = 0; i < numberOfVisitors; i++)
		{
			if(strcmp(visitors[i].eventID, visitors[numberOfVisitors - 1].eventID) == 0 && strcmp(visitors[i].name, "###") != 0)
			{
				place++;
			}
		}
		printf("On a(z) %d. jelentkezo a esemnyre.\n", place);
		int input;
		printf("\nAdjon meg egy szamot a folytatashoz... ");
		scanf("%d", &input);
	}
}

void menuListvisitors()
{
	system("clear");
	printf("Vendegek kilistazasa\n\n");
	int i;
	for(i = 0; i < numberOfVisitors; i++)
	{
		if(strcmp(visitors[i].name, "###") != 0)
		{
			printf("Nev: %s, E-mail: %s, Event: %s, Reg. Ido: %s\n", visitors[i].name, visitors[i].eMailAddress, visitors[i].eventID, visitors[i].registrationTime);
		}
	}
	int input;
	printf("\nAdjon meg egy szamot a folytatashoz... ");
	scanf("%d", &input);
}

void menuModifyVisitorData()
{
	system("clear");
	printf("Vendeg adatainak modositasa\n\n");
	int i;
	for(i = 0; i < numberOfVisitors; i++)
	{
		if(strcmp(visitors[i].name, "###") != 0)
		{
			printf("%d - Nev: %s, E-mail: %s, Event: %s, Reg. Ido: %s\n", i+1, visitors[i].name, visitors[i].eMailAddress, visitors[i].eventID, visitors[i].registrationTime);
		}
	}
	printf("0 - Kilepes\n\n");
	int input;
	printf("Adjon meg egy szamot: ");
	scanf("%d", &input);
	if(input > 0 && input < numberOfVisitors+1 && strcmp(visitors[input - 1].name, "###") != 0)
	{
		printf("Adja meg a vendeg uj nevet: ");
		scanf("%s", visitors[input - 1].name);
		printf("Adja meg a vendeg uj e-mail cimet: ");
		scanf("%s", visitors[input - 1].eMailAddress);
	}
}

void menuRemoveVisitor()
{
	system("clear");
	printf("Vendeg torlese\n\n");
	int i;
	for(i = 0; i < numberOfVisitors; i++)
	{
		if(strcmp(visitors[i].name, "###") != 0)
		{
			printf("%d - Nev: %s, E-mail: %s, Event: %s, Reg. Ido: %s\n", i+1, visitors[i].name, visitors[i].eMailAddress, visitors[i].eventID, visitors[i].registrationTime);
		}
	}
	printf("0 - Kilepes\n\n");
	int input;
	printf("Adjon meg egy szamot: ");
	scanf("%d", &input);
	if(input > 0 && input < numberOfVisitors+1 && strcmp(visitors[input - 1].name, "###") != 0)
	{
		strcpy(visitors[input - 1].name, "###");
	}
}

void menuNewEvent()
{
	system("clear");
	printf("Uj esemeny felvetele\n\n");
	printf("1 - Felvesz egy uj esemenyt\n");
	printf("0 - Kilepes\n\n");
	int input;
	printf("Adjon meg egy szamot: ");
	scanf("%d", &input);
	if(input == 1)
	{
		printf("Aktiv esemenyek:\n");
		int i;
		for(i = 0; i < numberOfEvents; i++)
		{
			if(strcmp(events[i].id, "###") != 0)
			{
				printf("Id: %s, Nev: %s\n", events[i].id, events[i].name);
			}
		}
		struct Event pr;
		int response = 0;
		do
		{
			printf("\nAdja meg az esemeny azonositojat: ");
			scanf("%s", pr.id);
			printf("Adja meg az esemeny nevet: ");
			scanf("%s", pr.name);
			response = iseventIDPresent(pr.id);
		} while(response == 1);
		events[numberOfEvents] = pr;
		numberOfEvents++;
	}
}

void menuRemoveEvent()
{
	system("clear");
	printf("Program torlese\n\n");
	int i;
	for(i = 0; i < numberOfEvents; i++)
	{
		if(strcmp(events[i].id, "###") != 0)
		{
			printf("%d - Id: %s, Nev: %s\n", i+1, events[i].id, events[i].name);
		}
	}
	printf("0 - Kilepes\n\n");
	int input;
	printf("Adjon meg egy szamot: ");
	scanf("%d", &input);
	if(input > 0 && input < numberOfEvents+1 && strcmp(events[input - 1].id, "###") != 0)
	{
		int j;
		for(j = 0; j < numberOfVisitors; j++)
		{
			if(strcmp(visitors[j].eventID, events[input-1].id) == 0)
			{
				strcpy(visitors[j].name, "###");
			}
		}
		strcpy(events[input-1].id, "###");
	}
}

int exitEvent()
{
	printf("Kilepes...\n");
	return 0;
}

void runEvent()
{
	int isRunning = 1;
	while(isRunning == 1)
	{
		system("clear");
		printMenu();
		int input;
		printf("Adjon meg egy szamot: ");
		scanf("%d", &input);
		switch(input)
		{
			case 0:
				isRunning = exitEvent();
				break;
			case 1:
				menuRegisterToEvent();
				break;
			case 2:
				menuListvisitors();
				break;
			case 3:
				menuModifyVisitorData();
				break;
			case 4:
				menuRemoveVisitor();
				break;
			case 5:
				menuNewEvent();
				break;
			case 6:
				menuRemoveEvent();
				break;
			default:
				isRunning = exitEvent();
				break;
		}
	}
}

int main(int argc,char** argv)
{
	readFromFiles();
	runEvent();
	writeToFile();
	return 0;
}
