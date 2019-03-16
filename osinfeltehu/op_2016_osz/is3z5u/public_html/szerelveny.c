#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

struct msz
{
	int hanyadik;
	int mennyivan;
};

struct msz_to_read
{
	int hanyadik;
	int mennyivan;
};

void handler(int x)
{
    x = x;
}

int main(int argc, char *argv[])
{		
		//int szerelvenyszam =10;
		int szerelvenyszam = atoi(argv[1]);
		struct msz egyik;
		struct msz masik;
        
		struct sigaction sigact;
        sigact.sa_handler = handler;
        sigemptyset(&sigact.sa_mask);
        sigact.sa_flags = 0;
        sigaction(SIGUSR1, &sigact, NULL);
		


        //char names[10][50];
        //char namesOriginal[10][50];
        //int i = 0;
        //int size = 0;
        //int missed;
        //char missedNames[10][50];

        int fd[2];
        char buff[300];

        char line[50];

        int Pipe = pipe(fd);
        if (Pipe < 0)
        {
            perror("Pipe error!");
            exit(-1);
        }

        pid_t child = fork();
        if (child < 0)
        {
            perror("fork() error!");
            exit(-1);
        }

        /* parent process */
        if (0 != child)
        {
            sigset_t sigset;
            sigfillset(&sigset);
            sigdelset(&sigset, SIGUSR1);
			
			egyik.hanyadik = (rand() % szerelvenyszam);
			egyik.mennyivan = szerelvenyszam;

            write(fd[1], &egyik, sizeof(egyik));

            if (sigsuspend(&sigset) == -1)
            {
                printf("A metro szerelveny elindult!\n");
            };
			int lol=0;
            write(fd[1], &lol, sizeof lol);
            sigsuspend(&sigset);

            wait(NULL);
			int mitortent;

            read(fd[0], &mitortent, sizeof mitortent);
            if (mitortent==1)
			{
				printf("Hat ezt be kellett vontatni!\n");
			}
			else
			{
				printf("Vegre valami jo szerelveny!\n");
			}
        }
        /* child process */
        else
        {
            memset(buff, 0, strlen(buff));
            read(fd[0], &masik, sizeof(masik));
            kill(getppid(), SIGUSR1);
            printf("A szerelveny megkapta a parametereket: %s\n", masik.hanyadik);
			int macska;
            read(fd[0], &macska, sizeof macska);
            kill(getppid(), SIGUSR1);
            //printf("Itt amugy nem irtam ki semmit\n");

            srand(time(NULL));
            int elcsuszott=0;
                if (rand() % 10 == 0)
                {
                    printf("Elcsusztam a bananhejon!\n");
					elcsuszott=1;
                }
                else
                {
                    elcsuszott=0;
                }
            

            write(fd[1], &elcsuszott, sizeof elcsuszott);
            exit(0);
            /*****************************************************/
        }
}		