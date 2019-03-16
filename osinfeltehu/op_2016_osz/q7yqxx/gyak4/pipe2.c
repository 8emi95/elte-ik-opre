#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void handler(){}

int main(int argc, char *argv[]){
	
	signal(SIGTERM,handler);
	
	int pipefd1[2];
	int pipefd2[2];
	
	pid_t pid;
	char sz[100];

	if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1) {
		perror("Hiba pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}

	pid = fork();

	if (pid == -1) {
		perror("Fork hiba");
		exit(EXIT_FAILURE);
	}

	//gyerek
	if (pid == 0) {
		close(pipefd1[1]);
		close(pipefd2[0]);
		
		read(pipefd1[0], sz, sizeof(sz));
		printf("Ezt az azonositot kaptam: %s\n",sz);
		
		//Jelzés
		kill(getppid(),SIGTERM); 
		
		read(pipefd1[0], sz, sizeof(sz));
		printf("Nekik kellene itt lenni: %s\n",sz);
		close(pipefd1[0]);
		
		//Sikeresség, kik nincsenek itt - > adatok elküldése
		char adatok[300];
		char nemJelentekMeg[200];
		
		strcpy(adatok,  "67%\n");
		strcpy(nemJelentekMeg, "Jozsi\n");

		strcat(adatok, nemJelentekMeg);
		
		write(pipefd2[1], adatok,sizeof(adatok));
		fflush(NULL);
		close(pipefd2[1]);
		
	}

	//szülő
	else {
		printf("Szulo indul!\n");
		
		close(pipefd1[0]);
		close(pipefd2[1]);
		
		//Eseményazonosító elküldése
		write(pipefd1[1], "123",4);
		fflush(NULL);
		
		//Signalra vár
		pause();
		write(pipefd1[1], "Joska, Pista, Laci",20);
		fflush(NULL);
		close(pipefd1[1]);
		
		int i;
		while ((i = read(pipefd2[0], sz, sizeof(sz))) > 0) {
				printf("%s\n",sz);
		}
		close(pipefd2[0]);
		
		wait();
		printf("Szulo befejezte!\n");	
	}
	
	exit(EXIT_SUCCESS);
}

 
