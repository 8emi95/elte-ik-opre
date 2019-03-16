//Hausknecht Bálint (b2icwh)
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <mqueue.h>
#include <signal.h>

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
		printf("Fajl megnyitÃ¡si hiba, events.txt, iras!\n");
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
        printf("Fajl megnyitÃ¡si hiba, events.txt, iras!\n");
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
		printf("Fajl megnyitÃ¡si hiba, events.txt, olvasas!\n");
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
        printf("7 - Esemeny lebonyolitasa\n");
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
int countNotMinus(int * array)
{
	int count = 0;
	int i;
	for(i = 0; i < sizeof(array) / sizeof(array[0]); i++)
	{
		if(array[i] != -1)
		{
			count++;
		}
	}
	return count;
}


void handler(int signumber){
	printf("Partner - Megerkeztem!\n", signumber);
}
void startProgram(int input)
{
	pid_t  pid;
        int fd[2];
	int fdVisitor[2];
	int fdRandom[2];
	
	int status;
	struct sigaction sigact;
	sigact.sa_handler=handler;
	sigaction(SIGUSR1,&sigact,NULL); 
	
	char * names[numberOfVisitors];
	struct Event event;
	
    if (pipe(fd) == -1)
    {
		perror("Hiba a pipe nyitaskor!");
		exit(1);
    }
	
    if (pipe(fdVisitor) == -1)
    {
		perror("Hiba a pipe nyitaskor!");
		exit(1);
    }
	
    if (pipe(fdRandom) == -1)
    {
		perror("Hiba a pipe nyitaskor!");
		exit(1);
    }
	
    pid = fork();
	
	if(pid < 0)
	{
		printf("Hiba tortent a folyamatletrehozas soran! A program leall...");
		exit(1);
	}
	else if (pid == 0) //child
	{	
		close(fd[1]);
		close(fdVisitor[1]);
		close(fdRandom[0]);
		
		read(fd[0], &event, sizeof(struct Event));
		int didNotCome[1000];

		int k;
		for(k = 0; k < 1000; ++k)
		{
			didNotCome[k] = -1;
		}

		printf("Partner - Megrendezett esemeny neve: %s\n", event.name);
		close(fd[0]);
		sleep(1);
		kill(getppid(),SIGUSR1);
		read(fdVisitor[0], &names, sizeof(names));
		sleep(1);
		printf("Partner - Akik jelentkeztek erre a rendezvenyre: ");

		srand(time(NULL));
		for(k = 0; k < (sizeof(names) / sizeof(names[0])); k++)
		{
			if(names[k] != NULL)
			{
				int r = rand() % 100 + 1;
				printf("%s ", names[k]);
				if(r > 90)
				{
					didNotCome[k] = k;
				}
			}
		}
		printf("\n");
		write(fdRandom[1], &didNotCome, sizeof(didNotCome));
		close(fdRandom[1]);
		close(fdVisitor[0]);
		printf("Partner - Rendezveny sikeresen levezenyelve\n");
		exit(0);
	}
	else //parent
	{		
		int nameIndex[1000];
		
		close(fd[0]);
		close(fdVisitor[0]);
		close(fdRandom[1]);
		printf("Szervezo - Elkuldom a partnernek a rendezveny adatait\n");
		write(fd[1], &(events[input - 1]), sizeof(events[input - 1]));
		close(fd[1]);
		
		sigset_t sigset;
		sigfillset(&sigset);
		sigdelset(&sigset,SIGUSR1);
		sigsuspend(&sigset);
		printf("Szervezo - Partner megerkezett\n");
		
		int count = 0;
		int i;
		for(i = 0; i < numberOfVisitors; ++i)
		{
			names[i] = NULL;
		}
		
		for(i=0; i < numberOfVisitors; ++i)
		{
			if(strcmp(events[input - 1].id, visitors[i].eventID) == 0 && strcmp("###", visitors[i].name) != 0)
			{
				names[count] = visitors[i].name;
				count++;
			}

		}
		
		write(fdVisitor[1],&names, sizeof(names));
		close(fdVisitor[1]);
		printf("Szervezo - Elkuldom a partnernek a rendezvenyen resztvevok nevet\n");
		
		read(fdRandom[0],&nameIndex, sizeof(nameIndex));
		close(fdRandom[0]);
		
		waitpid(pid, &status, 0);
		
		if(count == 0)
		{
			printf("Szervezo - A rendezvenyre nem jelentkezett senki!\n");
		}
		else
		{
			printf("Szervezo - Megjelenesi szazalek: %d%\n", (count * 100 - countNotMinus (nameIndex) * 100) / count );
		}
		printf("Szervezo - Aki nem jott el: \n");
		for(i = 0; i < (sizeof(nameIndex) / sizeof(int)); ++i)
		{
			if(nameIndex[i] != -1)
			{
				printf("%s\n", names[nameIndex[i]]);
			}
		}
	}
}
void menuStartProgram()
{
	system("clear");
	printf("Program indítása\n\n");
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
	printf("Adjon meg egy esemeny sorszamot: ");
	scanf("%d", &input);
	if(input > 0 && input < numberOfEvents+1 && strcmp(events[input - 1].id, "###") != 0)
	{
		startProgram(input);
	}
	int inputExit;
	printf("\nAdjon meg egy szamot a folytatáshoz... ");
	scanf("%d", &inputExit);
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
                        case 7: 
                               menuStartProgram();
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
