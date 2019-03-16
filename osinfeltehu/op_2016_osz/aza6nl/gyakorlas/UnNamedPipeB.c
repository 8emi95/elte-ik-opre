#include <unistd.h>	//fork, pipe
#include <stdio.h>	//Standard IO
#include <sys/types.h> //getpid
#include <stdlib.h> //exit
//Újak
#include <string.h>


int main(int argc, char* argv[])
{
	int n = 3;
	pid_t childs[n];
	pid_t parentPid = getpid();
    printf( "[Szülő] pid: %d - Nem nevesített cső példa - \n",  parentPid);
	printf("[Szülő] pid: %d - indul!\n", parentPid);
	
	//unnamed pipe file descriptor array
	int pipefd[2];
	char message[100];
	
	if (pipe(pipefd) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	int i;
    for (i = 0; i < n; i++)
	{
		pid_t pid = fork();
		if (pid == -1) 
		{
			perror("[HIBA] - Fork hiba\n");
			exit(EXIT_FAILURE);
		}
		if(getpid() == parentPid) childs[i] = pid;
		
        if (pid == 0)
        {
			pid_t childPid = getpid();
			pid_t myParentPid = getppid();
            printf("[gyerek] pid: %d - szülő: %d\n", childPid, myParentPid);
			
			sleep(3);
			//Usually we close the unused write end
			close(pipefd[1]);
			printf("[gyerek] pid: %d - Olvasás csőből!\n", childPid);
			//reading max 100 chars
			read(pipefd[0], message, sizeof(message));
			printf("[gyerek] pid: %d - Ezt olvastam: %s\n", childPid, message);
			// finally we close the used read en
			close(pipefd[0]);
			
			printf("[gyerek] pid: %d - leállok\n", childPid);
            exit(0);
        }
	}
	
    for ( i = 0; i < n; i++ )
	{
		//Usually we close unused read end
		close(pipefd[0]);
		printf("[Szülő] pid: %d - Csőbe ír: Hajra Fradi!", parentPid);
		write(pipefd[1], "Hajra Fradi!",13);
		printf("\n");
		// Closing write descriptor 
		close(pipefd[1]);
		printf("[Szülő] pid: %d - beírta az adatokat a csőbe!\n", parentPid);
		// flushes all write buffers (not necessary)
		fflush(NULL);
	}
	
	for ( i = 0; i < n; i++ )
	{
		waitpid( childs[i] );
	}
	
	printf("[Szülő] leáll\n");
}

