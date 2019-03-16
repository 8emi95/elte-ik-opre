#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

typedef struct
{
	int num;
	int total;
} vonal;

int pipefd[2];
int pipefd2[2];

void signal_handler(int signum)
{
	char buf[100];
	
	switch (signum)
	{
		case SIGUSR1:
			printf("[Gyerek]Sikeresen beert a szerelveny!\n");
			break;
		case SIGUSR2:
			read(pipefd2[0], &buf, sizeof(buf));
			printf("[Gyerek]Be kellett vontatni a szerelvenyt, mert %s.\n", buf);
			break;	
	}
	
	close(pipefd2[0]);
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	
	if (argc != 2)
	{
		printf("Hianyzo parameter!\n");
		exit(1);
	}
	
	if (pipe(pipefd) < 0 || pipe(pipefd2) < 0)
    {
        perror("Hiba a pipe nyitaskor!");
        exit(1);
    }

	pid_t childpid = fork();
	
	if (childpid < 0)
	{
		printf("Nem sikerult forkolni!\n");
		exit(1);
	}
	
	if (childpid == 0) // child
	{
		close(pipefd[1]);
		close(pipefd2[0]);

		vonal l_info_c;
		read(pipefd[0], &l_info_c, sizeof(l_info_c));
		
		int rate = rand() % 100 + 1;
		bool success = true;
		char reason[100];
		if (rate <= 10)			// 10% banánhéj
		{
			success = false;
			strcpy(reason, "elcsuszott egy bananhejon");
		}
		else if (rate <= 20)	// 10% zord tél
		{
			success = false;
			strcpy(reason, "lefagyott a zord telben");
		}
			
		printf("[Gyerek]Ez a(z) %d. szerelveny, meg %d db lesz ezutan, %s teljesitette az uzemet.\n",
			l_info_c.num,						// sorszam
			l_info_c.total - l_info_c.num,		// ennyi maradt
			(success ? "sikeresen" : "nem")		// sikerült vagy sem
		);
		
		// SIGUSR1 - sikerült
		// SIGUSR2 - nem sikerült
		kill(getppid(), (success ? SIGUSR1 : SIGUSR2));
		
		write(pipefd2[1], &reason, sizeof(reason));
		
		close(pipefd[0]);
		close(pipefd2[1]);
	}
	else // parent
	{
		close(pipefd[0]);
		close(pipefd2[1]);
		
		int count = atoi(argv[1]);
	
		vonal l_info;
		l_info.num = rand() % count + 1;	// szerelvény sorszám
		l_info.total = count;				// max szerelvényszám
		
		write(pipefd[1], &l_info, sizeof(vonal));
		
		struct sigaction sigact;
		sigemptyset(&sigact.sa_mask);
		sigact.sa_handler = signal_handler;
		sigact.sa_flags = 0;
		sigaction(SIGUSR1, &sigact, NULL);
		sigaction(SIGUSR2, &sigact, NULL);

		sigset_t sigmask;
		sigfillset(&sigmask);
		sigdelset(&sigmask, SIGUSR1);
		sigdelset(&sigmask, SIGUSR2);

		sigsuspend(&sigmask);

		close(pipefd[1]);
	}
}