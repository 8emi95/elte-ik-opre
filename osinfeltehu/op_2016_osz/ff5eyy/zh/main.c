#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>

#define BUFFER_SIZE 100
//call it with some parameters from the command line 

void handler(int signumber){
  //printf("Signal with number %i has arrived\n",signumber);
}

struct versenyzo_t
{
	char feladat[BUFFER_SIZE];
	char nev[BUFFER_SIZE];
	int sorszam;
};

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{	

	signal(SIGTERM,handler);
	int pipefd[2]; // unnamed pipe file descriptor array
	pid_t pid;
	if (pipe(pipefd) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	int i;
	for (i=0;i<argc-1;i++)
	{
		pid = fork();	// creating parent-child processes
		if (pid == -1) 
		{
			perror("Fork hiba");
			exit(EXIT_FAILURE);
		}

		if (pid == 0) 
		{		    	// child process
			struct versenyzo_t versenyzo;
			srand(time(NULL));
			int megoldva = rand()%2;
			//sleep(3);	// sleeping a few seconds, not necessary
			//close(pipefd[1]);  //Usually we close the unused write end
			printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
			read(pipefd[0],&versenyzo,sizeof(versenyzo)); 
			printf("Versenyzo neve: %s\n",versenyzo.nev);
			printf("Versenyzo sorszama: %d\n",versenyzo.sorszam);
			printf("Versenyzo feladata: %s\n",versenyzo.feladat);
			printf("\n");
			close(pipefd[0]); // finally we close the used read end
			
			kill(getppid(),SIGTERM);
			if(megoldva == 1){
				printf("Versenyzo megoldotta a feladatot\n");
				char megoldva_str[] = "Megoldva"; 
				write(pipefd[1], megoldva_str, sizeof megoldva_str);
			}
			else{
				printf("Versenyzo nem oldotta meg a feladatot\n");
				char megoldva_str[] ="Nincs megoldva"; 
				write(pipefd[1], megoldva_str, sizeof megoldva_str);
			}
			close(pipefd[1]);
			
		} 
		else 
		{    // szulo process
			char sorszam_str[10] = "0";
			char megoldva_str[BUFFER_SIZE];
			struct versenyzo_t versenyzo;
			strcpy(versenyzo.feladat, argv[i+1]);
			sprintf(sorszam_str, "%d", i+1);
			strcat(sorszam_str, ". versenyzo");
			strcpy(versenyzo.nev, sorszam_str );
			versenyzo.sorszam = i;
			
			printf("Tréner indul!\n");
			//close(pipefd[0]); //Usually we close unused read end
			write(pipefd[1], &versenyzo, sizeof(versenyzo));
			close(pipefd[1]); // Closing write descriptor 
			printf("Szulo beirta az adatokat a csobe!\n");
			//fflush(NULL); 	// flushes all write buffers (not necessary)
			//wait();		// waiting for child process (not necessary)
			
			pause(); //waits till a signal arrive 
			read(pipefd[0],&megoldva_str,sizeof(megoldva_str));
			if(strcmp(megoldva_str, "Megoldva") == 0)
			{
				printf("Tréner: Versenyzo megoldotta a feladatot\n");
			}
			else
			{
				printf("Tréner: Versenyzo nem oldotta meg a feladatot\n");
			}
			
			printf("Szulo befejezte!");	
		}
	}
	
	return 0;
}