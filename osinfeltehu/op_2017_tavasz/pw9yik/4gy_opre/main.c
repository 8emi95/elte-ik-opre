#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber)
{
}

int main(int argc, char *argv[])
{
    int pipefd[2]; // unnamed pipe file descriptor array
    pid_t pid;
    int sz;  // char array for reading from pipe
	
    signal(SIGTERM, handler);

    if (pipe(pipefd) == -1)
    {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }
    pid = fork();       // creating parent-child processes
    if (pid == -1)
    {
        perror("Fork hiba");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
		printf("gyerek indul!\n");
        kill(getppid(), SIGTERM);
		int i;
		close(pipefd[1]);  //Usually we close the unused write end
		printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
		read(pipefd[0], &sz, sizeof(sz)); // reading max 100 chars
		printf("Gyerek olvasta uzenet: %i", sz);
		printf("\n");
		close(pipefd[0]); // finally we close the used read end
    }
    else
    {    // szulo process
		pause();
        printf("Szulo indul!\n");
		int i;
		char tmp[2000];
		int voterCount;
		scanf("%d", &voterCount);
		printf("%i\n", voterCount);
		
		for(i = 1; i < voterCount; i++)
		{
			int random = rand();
			printf("%i\n", random);
		}
		close(pipefd[0]); //Usually we close unused read end
		write(pipefd[1], &random, 2000);
		close(pipefd[1]); // Closing write descriptor
		printf("Szulo beirta az adatokat a csobe!\n");
		fflush(NULL);   // flushes all write buffers (not necessary)
        wait();         // waiting for child process (not necessary)
        // try it without wait()
        printf("Szulo befejezte!");
    }
    exit(EXIT_SUCCESS); // force exit, not necessary
}


