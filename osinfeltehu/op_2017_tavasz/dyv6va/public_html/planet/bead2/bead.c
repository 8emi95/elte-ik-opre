#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
	

char* getDataByID(int ID, FILE *f)
{
	char line[300];
	int currID;
	while (!feof(f))
	{
		char *ret = line;
		fgets(line, sizeof(line), f);
		currID = atoi(line);
		if (currID == ID)
			return ret;
	}
	return NULL;
}

int countLinesByID(int ID, FILE *f)
{
	char line[300];
	int currID;
	int count = 0;
	while(fgets(line, sizeof(line), f))
	{
		currID = atoi(line);
		if (currID == ID)
			++count;
	}
	return count;
}


char* listAssignees(int eventID)
{
	char tmp[1000];
	int tmpi = 0;
	while (tmpi<1000)
	{
		tmp[tmpi] = '\0';
		++tmpi;
	}
	FILE *visits, *visitor;
	visits = fopen("VISITS.TXT","r");
	if (visits == NULL)
	{
		perror("Hiba a VISITS.TXT megnyitasa kozben!\n");
		exit(1);
	}
	char line[100];
	while (!feof(visits))
	{
		fgets(line, sizeof(line), visits);
		char *token = strtok(line, "\t");
		int currEvent = atoi(token);
		token = strtok(NULL, "\t");
		if (token == NULL) break;
		int currVisitor = atoi(token);
		if ( currEvent == eventID )
		{
			visitor = fopen("VISITOR.TXT","r");
			if (visitor == NULL)
			{
				perror("Hiba a VISITOR.TXT megnyitasa kozben!\n");
				fclose(visits);
				exit(1);
			}
			char *visitorData = getDataByID(currVisitor, visitor);
			int i=0;
			while (*(visitorData+i) != '\t') ++i;
			++i;
			char name[100];
			int j=0;
			while (*(visitorData+i) != '\t')
			{
				name[j] = *(visitorData+i);
				++j;
				++i;
			}
			name[j] = '\0';
			strcat(tmp, name);
			strcat(tmp, "\t");
			fclose(visitor);
		}
	}
	fclose(visits);
	char *ret = tmp;
	return ret;
}

void handler(int signumber)
{

}

int main()
{
	printf(" ------ King of Stands lebonyolitas ------\n");
	signal(SIGRTMIN, handler);
	int status;
	int pipefd[2];
	char sz[300];
	char eventData[4][300];
	char line[300];
	int go = 1;
	FILE *events = fopen("EVENT.TXT", "r");
	while (go != 1);
	while (fgets(line, sizeof(line), events))
	{
		if (pipe(pipefd) == -1)
		{
			perror("Hiba a pipe nyitaskor!");
			exit(EXIT_FAILURE);
		}
		pid_t child = fork();
		if (child < 0)
		{
			perror("Hiba a gyerekfolyamat letrehozasa kozben!\n");
			exit(1);
		}
		if (child > 0)
		{
			printf("Szulofolyamat elindult!\n");

			// EVENT.TXT megnyitasa es a rendezveny kiolvasasa

			
			if (events == NULL)
			{
				perror("Hiba az EVENT.TXT fajl megnyitasa kozben!");
				exit(1);
			}
			int ei = 0;
			while (ei < 300)
			{
				eventData[3][ei] = '\0';
				++ei;
			}
			char *eventTok = strtok(line, "\t");
			int i=0;
			while(eventTok != NULL)
			{
				strcpy(eventData[i], eventTok);
				eventTok = strtok(NULL, "\t");
				++i;
			}
			fclose(events);

			// A rendezveny helyszinenek bepakolasa a csobe

			write(pipefd[1], eventData[3], strlen(eventData[3])-1);

			// Gyerekfolyamat bevarasa

			pause();
			printf("Szulo: megkaptam a gyerekfolyamat jelzeset!\n");

			// A rendezveny resztvevoinek bepakolasa a csobe

			write(pipefd[1], eventData[0], strlen(eventData[0]));

			// Kiolvassuk, mennyire volt sikeres a rendezveny

			pause();
			int success;
			read(pipefd[0], &success, sizeof(success));
			printf("Szulo: a rendezveny sikeressege: %i \%\n", success);

			// Kiolvassuk, hogy milyen hosszu string fog erkezni

			// pause();
			int strLength;
			read(pipefd[0], &strLength, sizeof(strLength));

			// Vegul kiolvassuk, ki nem jott el (ha volt egyaltalan ilyen)

			if (strLength == 0)
			{
				printf("Szulo: a rendezvenyre mindenki eljott!\n");
			}
			else
			{
				char notPresent[1000];
				read(pipefd[0], notPresent, strLength);
				notPresent[strLength+1] = '\0';
				printf("Szulo: a rendezvenyen nem vett reszt: %s\n", notPresent);
			}

			waitpid(child, &status, 0);
			printf("Szulo: a gyerekfolyamat veget ert!\n");
			close(pipefd[1]);
			close(pipefd[0]);
		}
		else
		{
			go = 0;
			// A gyerek kiolvassa csobol, hogy hova kell mennie

			read(pipefd[0], sz, sizeof(eventData[3]));
			printf("Gyerek: Elindultam ide: %s\n", sz);

			// A gyerek ertesiti a szulot, hogy odaert

			printf("Gyerek: Megerkeztem!\n");
			kill(getppid(), SIGRTMIN);

			// A rendezveny resztvevoinek listazasa

			read(pipefd[0], sz, sizeof(eventData[0]));
			printf("Gyerek: megkaptam a szulotol a resztvevoket:\n");
			int eventID = atoi(sz);
			char *assignees = listAssignees(eventID);
			printf("%s\n", assignees);
			FILE *visits = fopen("VISITS.TXT", "r");
			int participants = countLinesByID(eventID, visits);
			fclose(visits);
			printf("Gyerek: osszesen %i embert varunk a rendezvenyre.\n", participants);
			printf("Gyerek: megvarom a rendezveny veget...\n");

			// A rendezveny lebonyolitasa

			// Hany szazalekban volt sikeres a rendezveny

			srand(time(NULL));
			int success = rand() % 101;
			write(pipefd[1], &success, sizeof(success));
			kill(getppid(), SIGRTMIN);

			// Hanyan nem erkeztek meg

			int arrived = 0;
			char name[100];
			char notPresent[1000];
			notPresent[0] = '\0';
			int j = 0;
			int k = 0;
			int i = 0;
			while (i < participants)
			{
				if(rand() % 9 != 0)
				{
					while (*(assignees+j) != '\t') ++j;
					++j;
					++arrived;
				}
				else
				{
					while(*(assignees+j) != '\t')
					{
						notPresent[k] = *(assignees+j);
						++k;
						++j;
					}
					notPresent[k] = '\t';
					++k;
					++j;
				}
				++i;
			}
			notPresent[k] = '\0';
			printf("Gyerek: ennyien voltak a rendezvenyen: %i\n", arrived);

			// Eloszor azt tesszuk bele a csobe, hogy a szulo milyen hosszu string-et varjon

			int strLength = strlen(notPresent);
			write(pipefd[1], &strLength, sizeof(strLength));
			// kill(getppid(), SIGRTMIN);

			if (strLength != 0)
			{
				write(pipefd[1], notPresent, strLength);
			}

			close(pipefd[0]);
			close(pipefd[1]);
			go = 1;
		}
	}
	return 0;
}
