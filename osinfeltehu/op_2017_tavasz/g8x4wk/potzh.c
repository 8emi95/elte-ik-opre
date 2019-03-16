#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


int main(int argc, char* argv[])
{
	int tszam;
	int szul;
	int status = 0;

	int pipefd[2];
	
	pid_t ugynok;

	if (pipe(pipefd) == -1)
    {
            perror("Pipe hiba keletkezett!\n");
            exit(EXIT_FAILURE);
    }


	
	if (ugynok == -1)
    {
        perror("Fork(ugynok) hiba");
        exit(EXIT_FAILURE);
    }

	ugynok = fork();
	if(ugynok>0)
	{
		pid_t vasarlo=fork();
		if (vasarlo == -1)
			{
				perror("Fork(vasarlo) hiba");
				exit(EXIT_FAILURE);
			}
			
		if (vasarlo >0)
		{
			wait();
		}
		else
		{
			printf("(vasarlo)tszam: %i\n", getpid());	
			printf("(vasarlo)Hívas fogadasa!\n");	
		}
		
		
		//printf("(vallalat) a vasalo telefonszama : %i \n", vasarlo);
		szul=vasarlo;
		//write(pipefd[0], szul, sizeof(szul));
		close(pipefd[1]);
        close(pipefd[0]);
		wait();
	}
	else 
    {
		//read(pipefd[1],tszam,sizeof(tszam));
		printf("(ugynok)vasarlo hívasa :  %i \n",tszam);
        close(pipefd[0]);
        close(pipefd[1]);

	}


	



}