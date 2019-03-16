#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>  
#include <errno.h> 
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>


void handler(int signumber){
	//printf("A %i szamu szignal megerkezett\n",signumber);
}

int main(int argc,char ** argv){
	

	signal(SIGUSR1, handler);
	
	int pipefd[2]; 
	
	pid_t pid;
	char sz[100];  
	int szam = 0;	
	
	if (pipe(pipefd) == -1){
	   perror("Hiba a pipe nyitaskor!");
	   exit(EXIT_FAILURE);
    } 
	
	int status;
	
	pid_t child=fork(); 
	
	if (child<0){
		error("Fork hiba");
        exit(EXIT_FAILURE);
	}
	
	if (child>0){
		printf("Szulo: megnyitom a jatekot! \n");
		char* feladat = "Fejtsd meg az ENIGMA-t!";
		char* nev = argv[1];
		szam += 1;

		write(pipefd[1], feladat, strlen(feladat)+1);
		pause();
		
		write(pipefd[1], nev, sizeof(nev));
	    pause();
		
		write(pipefd[1], &szam, sizeof(int));
		
		pause();

		read(pipefd[0], &szam,sizeof(int));
		
		if(szam < 5){
			printf("A versenyzo teljesitette a feladatot \n");
		} else {
			printf("A versenyzo nem tudta teljesiteni a feladatot \n");
		}
		
		
		
		close(pipefd[1]);
		close(pipefd[0]);
		
		wait(&status);
		
	}else{
		
		
		read(pipefd[0],sz,sizeof(sz));
		printf("A feladatom: %s ,", sz);
		
		kill(getppid(), SIGUSR1);
		
		read(pipefd[0],sz,sizeof(sz));
		printf("A nevem: %s ,", sz);
		
		kill(getppid(), SIGUSR1);		
		
		read(pipefd[0],&szam,sizeof(int));
		printf("A sorszamom: %i \n", szam);
			
		srand(time(NULL));
		int rnd = rand()%10 + 1;
		
		write(pipefd[1], &rnd, sizeof(int) );
		printf("A kuldott szam: %i \n", rnd);
		
		kill(getppid(), SIGUSR1);
		
		close(pipefd[0]);
		close(pipefd[1]);
		fflush(stdout);
		
	}
	return 0;
}