#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int hang = 1;

void handler(int signumber){
	// sleep(1);
  	printf("Signal with number %i has arrived in %i\n",signumber, getppid());
  	hang = 0;
}

int main(int argc, char const *argv[])
{
	if(argc < 2)
	{
		perror("Nincs megadva argumentum (Nev)!");
		exit(EXIT_FAILURE);
	}

	int pipefd[2];	// unnamed pipe file descriptor array
	pid_t pid;
    char sz[100];  // char array for reading from pipe

    signal(SIGUSR1,handler);
    int i, cntn = 0, j = 0, rc = 0;
    int repeat[argc-1];

    if (pipe(pipefd) == -1) 
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
    {		    					// child process
        sleep(2);
        printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");

        read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
        printf("Gyerek olvasta uzenet: %s\n",sz);
        kill(getppid(), SIGUSR1);
        
        read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
        printf("Gyerek olvasta uzenet: %s\n",sz);
        kill(getppid(), SIGUSR1);
        
        read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
        printf("Gyerek olvasta uzenet: %s\n",sz);
        close(pipefd[0]);
        
        srand(time(NULL));
		char result[14] = "Nem sikerult!";
		
		if(rand() % 100 < 50) strcpy(result,"Sikerult!");
        printf("Gyerek irja az eredmenyt: %s\n",result);
        write(pipefd[1], result, sizeof(result));
        close(pipefd[1]);
        kill(getppid(), SIGUSR1);
        // sleep(2);
    } 
    else 
    {    							// szulo process 
       	printf("Szulo indul!\n");
       	
       	write(pipefd[1], "Szulo ir a csobe feladatot!",28);
       	printf("Szulo beirta az adatokat a csobe!\n");
       	while(hang){sleep(1);}
       	hang = 1; 
       	
       	write(pipefd[1], argv[1], sizeof(argv[1]));
       	printf("Szulo beirta az adatokat a csobe!\n");
       	while(hang){sleep(1);}
       	hang = 1; 
       	
       	write(pipefd[1], "Szulo kuldi a sorszamot!",25);
       	printf("Szulo beirta az adatokat a csobe!\n");
       	close(pipefd[1]); // Closing write descriptor 
       	while(hang){sleep(1);}
       	hang = 1; 

       	read(pipefd[0], &sz, sizeof(sz));
       	close(pipefd[0]); // finally we close unused read cntn
       	printf("Szulo megkapta a valaszt: %s\n", sz);

       	if (strcmp("Nem sikerult!",sz) != 0 && j == 0)
       	{
       		repeat[rc++] = i;
       		cntn = 1;
       	} else if(j > 0)
       	{
       		if(++j > rc) cntn = 0;
       	}
       	
       	fflush(NULL); 	// flushes all write buffers (not necessary)
       	wait();		// waiting for child process (not necessary)
       	pid = fork();	// creating parent-child processes
	    if (pid == -1) 
	    {
	       	perror("Fork hiba");
	       	exit(EXIT_FAILURE);
	    }
	    write(pipefd[1], "Szulo ir a csobe feladatot!",28);
       	printf("Szulo beirta az adatokat a csobe!\n");
       	while(hang){sleep(1);}
       	hang = 1; 
       	
       	write(pipefd[1], argv[1], sizeof(argv[1]));
       	printf("Szulo beirta az adatokat a csobe!\n");
       	while(hang){sleep(1);}
       	hang = 1; 
       	
       	write(pipefd[1], "Szulo kuldi a sorszamot!",25);
       	printf("Szulo beirta az adatokat a csobe!\n");
       	close(pipefd[1]); // Closing write descriptor 
       	while(hang){sleep(1);}
       	hang = 1; 

       	read(pipefd[0], &sz, sizeof(sz));
       	close(pipefd[0]); // finally we close unused read cntn
       	printf("Szulo megkapta a valaszt: %s\n", sz);

       	fflush(NULL); 	// flushes all write buffers (not necessary)
       	wait();	

        printf("Szulo befejezte!\n");	
    }
	return 0;
}