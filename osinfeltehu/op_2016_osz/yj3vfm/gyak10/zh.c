// linekeléshez: -lrt

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>
#include <time.h>
#include <signal.h>

int pipefd[2];
char* mqname="/usor";
struct mq_attr attr;
mqd_t mq1, mq2;
char buffer[100];

void handler(int signumber)
{

}

int main (int argc, char* argv[])
{
	signal(SIGRTMIN, handler);
	
	int travel_time;
	int sh_mem_id = shmget(travel_time, sizeof(travel_time), NULL);
	
	int max_szerelveny = atoi(argv[1]);
    pid_t child;
    int uzenetsor, status;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = 50;
	
	int sum = 0;

    mq_unlink(mqname);
    mq1 = mq_open(mqname, O_CREAT | O_RDWR, 0600, &attr);
    if (mq1 < 0)
    {
        printf("Hiba az uzenetsor nyitasakor: %d\n", errno);
        return 1;
    }
	
	int i;
	int mentesito = 0;
	for (i = 1; i <= max_szerelveny; ++i)
	{
		child = fork();
		if (child > 0)
		{
			srand(getpid());
			//int szerelveny = rand() % max_szerelveny + 1;
			char szerelveny_ch[10];
			sprintf(szerelveny_ch, "%d", i);
			int db = mq_send(mq1, szerelveny_ch, strlen(szerelveny_ch), 5);
			db = mq_send(mq1, argv[1], strlen(argv[1]), 5);
			
			pause();
			
			int bi;
			for (bi = 0; bi<100; ++bi) buffer[bi] = '\0';
			db = mq_receive(mq1,buffer, 54, NULL);
			printf("Szulo: a szerelveny uzenete: %s\n", buffer);
			if (strstr(buffer,"sikeres") == NULL)
			{
				child = fork();
				if (child > 0)
				{
					char *msg = "POT";
					db = mq_send(mq1, msg, strlen(msg), 5);
					db = mq_send(mq1, szerelveny_ch, strlen(szerelveny_ch), 5);
					++mentesito;
					
					pause();
					int bi;
					for (bi = 0; bi<100; ++bi) buffer[bi] = '\0';
					db = mq_receive(mq1,buffer, 54, NULL);
					printf("Szulo: a POTszerelveny uzenete: %s\n", buffer);
				}
				else
				{
					// Kiolvass az üzenetet a sorból
	int pot = 0;
    int db = mq_receive(mq1, buffer, 54, NULL);
	if (strstr(buffer, "POT") != NULL)
	{
		printf("Gyerek: Potszerelveny vagyok!\n");
		db = mq_receive(mq1, buffer, 54, NULL);
		pot = 1;
	}
	
	int id = atoi(buffer);
	// Kisorsoljuk, hogy sikeres lesz-e az út
	
	srand(getpid());
	int success = rand() % 10;
	int loc_travel_time = -1;
	if (pot == 0)
	{
		db = mq_receive(mq1,buffer, 54, NULL);
		int kapott_max = atoi(buffer);
		printf("Gyerek: En vagyok a %d. szerelveny, meg %d lesz utanam.\n", id, kapott_max - id);
		loc_travel_time = rand() % 21 + 10;
	}
	else
	{
		printf("Gyerek: A %d. szerelvenyt potlom...\n", id);
	}
	char *message;
	if (success != 0)
	{
		message = "A szerelveny sikeresen beert.";
	}
	else
	{
		int reason = rand() % 2;
		if (reason == 0)
		{
			message = "A szerelveny elcsuszott egy bananhejon.";
		}
		else
		{
			message = "A szerelveny lefagyott a zord telben.";
		}
	}
	printf("Gyerek: %s\n", message);
	printf("Gyerek: az utazasi ido: %d\n", loc_travel_time);
	travel_time = loc_travel_time;
	
	kill(getppid(), SIGRTMIN);
	db = mq_send(mq1, message, strlen(message), 5);
	shmdt(travel_time);
    printf("Gyerek folyamat vege!\n");
	exit(1);
				}
			}
			else
			{
				sum += travel_time;
			}
		}
		else
		{
			// Kiolvass az üzenetet a sorból
	int pot = 0;
    int db = mq_receive(mq1, buffer, 54, NULL);
	if (strstr(buffer, "POT") != NULL)
	{
		printf("Gyerek: Potszerelveny vagyok!\n");
		db = mq_receive(mq1, buffer, 54, NULL);
		pot = 1;
	}
	
	int id = atoi(buffer);
	// Kisorsoljuk, hogy sikeres lesz-e az út
	
	srand(getpid());
	int success = rand() % 10;
	int loc_travel_time = -1;
	if (pot == 0)
	{
		db = mq_receive(mq1,buffer, 54, NULL);
		int kapott_max = atoi(buffer);
		printf("Gyerek: En vagyok a %d. szerelveny, meg %d lesz utanam.\n", id, kapott_max - id);
		loc_travel_time = rand() % 21 + 10;
	}
	else
	{
		printf("Gyerek: A %d. szerelvenyt potlom...\n", id);
	}
	char *message;
	if (success != 0)
	{
		message = "A szerelveny sikeresen beert.";
	}
	else
	{
		int reason = rand() % 2;
		if (reason == 0)
		{
			message = "A szerelveny elcsuszott egy bananhejon.";
		}
		else
		{
			message = "A szerelveny lefagyott a zord telben.";
		}
	}
	printf("Gyerek: %s\n", message);
	printf("Gyerek: az utazasi ido: %d\n", loc_travel_time);
	travel_time = loc_travel_time;
	
	kill(getppid(), SIGRTMIN);
	db = mq_send(mq1, message, strlen(message), 5);
	shmdt(travel_time);
    printf("Gyerek folyamat vege!\n");
	exit(1);
		}
	}
	wait(NULL);
	printf("A BMF tajekoztatoja: %d potjaratot kellett inditani.\n", mentesito);
	int j;
	printf("A BMF tajekoztatoja: Az atlagos menetido: %d\n", travel_time);
	shmdt(travel_time);
	mq_close(mq1);
	mq_unlink(mqname);
    return 0;
}