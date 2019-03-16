#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>


void handler(int signumber){
	printf("--------------\n");
	return;
}

int main (int argc,char* argv[]) {
	srand(time(NULL)); 
	const int maxCim = 20;
	char cim[maxCim];
	
	int pfd[2]; 
	
	if (argc < 2)
	{
	   perror("Nincs utas lakcim megadva!");
	   exit(EXIT_FAILURE);
	}
	
	struct sigaction sigact;
	sigact.sa_handler=handler;
	sigemptyset(&sigact.sa_mask); 
	sigact.sa_flags=0;
	sigaction(SIGUSR1,&sigact,NULL);
	
	sigset_t sigset;
	sigfillset(&sigset);
	sigdelset(&sigset,SIGUSR1);
	
	if (pipe(pfd) == -1)
	{
	   perror("Hiba pipe megnyitasakor!");
	   exit(EXIT_FAILURE);
	}
	
	pid_t pid = fork();	
	
	if (pid == -1)
	{
	   perror("FORK Hiba!");
	   exit(EXIT_FAILURE);
	}
	
	//központ
	if (pid != 0)
	{		
		pid_t pid2 = fork();
		if (pid2 == -1)
		{
		   perror("FORK Hiba!");
		   exit(EXIT_FAILURE);
		}
		//még mindig
		if (pid2 != 0)
		{
			sigsuspend(&sigset);
			printf("Uj utas jelentkezett a kozpontnal!\n");
			
			read(pfd[0], cim, sizeof(cim));
			printf("cime: %s\n", cim);
			
			kill(pid,SIGUSR1);
			write(pfd[1], cim, sizeof(cim));
			
			write(pfd[1], &pid2, sizeof(pid2));
			
			sigsuspend(&sigset);
			
			char taxis_uzenet[20];
			read(pfd[0], taxis_uzenet, sizeof(taxis_uzenet));
			printf("Sofor: %s\n", taxis_uzenet);
			
			wait(NULL);
		}else
		{ 
			//utas
			kill(getppid(),SIGUSR1); 
			write(pfd[1], argv[1],maxCim);	
		}	
	}else
	{    
		//taxi
		sigsuspend(&sigset); 
		read(pfd[0], cim, sizeof(cim));
		printf("Sofor megkapta a cimet: %s\n", cim);
		pid_t utas;
		read(pfd[0], &utas, sizeof(utas));		
		kill(getppid(),SIGUSR1);
		write(pfd[1], "Elindultam!", 12);
		
	 }

	return 0;
}













//------------------------
/*
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>

bool waiting = true;

void Handler(int signum)
{
	waiting = false;
	printf("Egy utas jelentkezett taxiert.\n");
}

int main(int argc,char* argv[])
{
	int pfd1[2], pfd2[2];
	int status;
	
	char* cim;
	
	cim = argv[1];
	
	if (argc != 2)                          // nincs megadva argumentum, hiba 
    {
		printf("hasznalat: utas <lakcim>\n");
		exit(1);
    }
    
    
}*/
