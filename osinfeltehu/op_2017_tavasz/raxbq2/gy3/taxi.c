#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>


int main(int argc, char *argv[])
{
    pid_t pid;
	int pipefd[2];
    char* uzenet = argv[1];  // pipe üzenetek tömbje

    if (pipe(pipefd) == -1)
	{
        perror("Problem with pipe opening!");
        exit(EXIT_FAILURE);
    }
    pid = fork();	// gyerekfolyamat létrehozása
    if (pid == -1)
	{
        perror("Problem with fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) // gyerek
	{		    	

        close(pipefd[0]); 
        printf("Utas jelentkezik !\n");
        write(pipefd[0],uzenet,sizeof(uzenet));
        printf("Az utas lakcime: %s\n",uzenet);
        printf("\n");
        close(pipefd[1]);
    }
    else
    {    
        printf("Kozpont varja az utasokat!\n");
        close(pipefd[1]);
        read(pipefd[0],uzenet,sizeof(uzenet));
        printf("Kozpont olvasta cim: ");
        printf("%s\n",uzenet);
        wait();
		printf("Szulo befejezte!");
	}
	exit(EXIT_SUCCESS);
}
