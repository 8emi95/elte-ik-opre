#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char* argv[])
{
    char cim[100];

    char szul[100];

	int status = 0;

	int pipefd[2];
	pid_t pid;

	if (pipe(pipefd) == -1)
    {
            perror("Pipe hiba keletkezett!\n");
            exit(EXIT_FAILURE);
    }

    if (pid == -1)
    {
        perror("Fork hiba");
        exit(EXIT_FAILURE);
    }

    pid=fork();


    if(pid == 0)
    {

        printf("Kerem adja meg a lakcimet, majd nyomjon egy entert: \n");
        scanf("%[^\n]", cim);
        close(pipefd[0]);
        write(pipefd[1], cim, sizeof(cim));


        close(pipefd[1]);
        printf("\n");
	}

	else
	{
        close(pipefd[1]);



		 read(pipefd[0],szul,sizeof(szul));
		 wait();
         close(pipefd[0]);
         printf("Az utas által megadott cim: %s \n", szul);

	}

}