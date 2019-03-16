#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void handler(int num){}
int main()
{
	srand(time(NULL));
	char childAr[90];
	int pipefd[2];
	signal(SIGUSR1,handler);

	if (pipe(pipefd) == -1) 
	{
	   perror("The pipe open has failed!");
	   exit(EXIT_FAILURE);
	}
	pid_t ellen = fork();
	if( ellen > 0)
	{
		pid_t pecset = fork();
		if( pecset > 0)
		{
			int i;
			for(i = 0; i < 4;++i)
			{
				write(pipefd[1],"belso gyerek",13);
				kill(ellen,SIGUSR1);
				sleep(3);
			}
		}
	}else{	
		int j;
		for(j = 0; j < 4; ++j)
		{
			pause();
			read(pipefd[0],childAr,sizeof(childAr));
			printf("%s\n",childAr);
			
		}
	}
}
