#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h>

int childpid=0;

void handler(int signumber){
	kill(childpid, 10);
}

int main(int argc, char* argv[]){
	time_t t;
	srand((unsigned) time(&t));

	if(argc<2){
		printf("Nincs megadva paraméter\n");
	}
	else{
		signal(SIGTERM, handler);		

		int pipefd[2];
		pid_t pid;
		char sz[256];
		int msg, status;


		if(pipe(pipefd) == -1){
			perror("Hiba a pipe nyitásakor");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1){
			perror("Hiba a fork létrehozásakor");
			exit(EXIT_FAILURE);
		}

		if (pid == 0){//versenyzo
			char message[256];
			read(pipefd[0], sz, sizeof(sz));
			printf(sz);
			printf("\n");
			int rnd = rand() % 2;
			if(rnd == 0){
				sprintf(message, "Nem sikerült megoldani a feladatot\n");
				printf(message);
				
				write(pipefd[1], message, 256);
				kill(getppid(), SIGTERM);
				//fflush(NULL);
				
			}
			else{
				sprintf(message, "Sikerült megoldani a feladatot\n");			
				printf(message);
				
				write(pipefd[1], message, 256);
				kill(getppid(), SIGTERM);
				//fflush(NULL);
				
			}
			close(pipefd[1]);
			close(pipefd[0]);
		}
		else{ //trener
			char msg2[256];
			childpid=pid;
			char message[256];
			int rnd = rand() % 50 + 1;
			sprintf(message, "nev: %s sorzsam: %d \n", argv[1], rnd);
			write(pipefd[1], message, 256);
			fflush(NULL);
			pause();
			printf("megjött a jelzés \n");
			read(pipefd[0], msg2, sizeof(msg2));
			printf(msg2);
			printf("\n");
		}
	}
	return 0;
}