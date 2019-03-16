#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>



int szamlalo;



void handler(int signumber)
{
    ++szamlalo;
}

void elnok()
{
    printf("elnok\n");
}

void pecsetelo()
{
    kill(getppid(), SIGUSR1);
    printf("pecsetelo\n");
}

void ellenor()
{
    kill(getppid(), SIGUSR1);
    printf("ellenor\n");
}




int main(int argc, char **argv)
{
    //1. feladat:
    srand(time(NULL));

    szamlalo = 0;

    struct sigaction sigact;
    sigact.sa_handler = handler;
    sigemptyset(&sigact.sa_mask);  //now only the arriving signal, SIGTERM will be blocked
    sigact.sa_flags = 0;
    sigaction(SIGUSR1,&sigact,NULL);

    pid_t ellenor_pid = fork();

    if (ellenor_pid > 0)
    {
        pid_t pecsetelo_pid = fork();
	if (pecsetelo_pid > 0)
	{
	    while (szamlalo < 2)
	    {
		usleep(1000);
	    }
	    elnok();
	    int szavazok_szama = atoi(argv[1]);
	    int *szavazok = (int*)malloc(szavazok_szama*sizeof(int));
	    int i;
	   for (i = 0; i < )
int r = rand();

	    wait(NULL);
	    wait(NULL);
	}
	else
	{
	    pecsetelo();
	}
    }
    else 
    {
	ellenor();
    }

    return 0;
}