#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
//
// unnamed pipe example
//
int main(int argc, char *argv[])
{
	// 2 file leiro kell a nevtelen csőhöz. 1 a cső elejére; 2 a cső végére
	int pipefd[2]; // unnamed pipe file descriptor array
	int pipefd2[2]; // unnamed pipe file descriptor array
	pid_t pid;
	char sz[100];  // char array for reading from pipe

	// -1 ha nem sikerült a pipe létrehozása
	// a csővezeték azonnal nyitva lesz és le kell zárni !!!!!!
	if (pipe(pipefd) == -1 || pipe(pipefd2) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
   pid = fork();	// creating parent-child processes
   if (pid == -1) 
	{
		perror("Fork hiba");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) 
	{
		// child process
		// HA NEM használjuk valamelyik végét a csőnek akkor azt le kell zárni
		// IRÓVÉG: [1]
		// OLVASÓVÉG: [0]
		sleep(3);	// sleeping a few seconds, not necessary
		
		close(pipefd[1]);  //Usually we close the unused write end
		close(pipefd2[0]);
		printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
		read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
		printf("Gyerek olvasta uzenet: %s",sz);
		printf("\n");
		
		
		write(pipefd2[1], "Nem szeretem a focit !!", 23);
		close(pipefd[0]);
		close(pipefd2[1]); // finally we close the used read end
	}
	else
	{ 
		//ha a szülő vár 3 mp -et akkor a gyerek folyamat megvárja a amíg a szülő nem kezd írni
		//sleep(3);
		// HA NEM használjuk valamelyik végét a csőnek akkor azt le kell zárni
		// szulo process 
		printf("Szulo indul!\n");
		close(pipefd2[1]);
		close(pipefd[0]); //Usually we close unused read end
		
		write(pipefd[1], "Hajra Fradi!",13);
		printf("Szulo beirta az adatokat a csobe!\n");
		
		read(pipefd2[0], sz, sizeof(sz));
		printf("Ez a válasz: %s \n", sz);
		close(pipefd[1]); // Closing write descriptor 
		close(pipefd2[0]);
		fflush(NULL); 	// flushes all write buffers (not necessary)
		wait();		// waiting for child process (not necessary)
		// try it without wait()
		printf("Szulo befejezte!");	
	}
	exit(EXIT_SUCCESS);	// force exit, not necessary
}


