#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>

int main(){
	
	int pipefd[2];
	
	pid_t pid;
	
	char sz[100];
	
	if(pipe(pipefd)==-1){               
		perror("Hiba a pipe nyitasakor!\n");
		exit(EXIT_FAILURE);
	}
	
	pid=fork();
	
	if(pid==-1){
		perror("Fork hiba\n");
		exit(EXIT_FAILURE);
	}
	
	//child
	if(pid==0){
		sleep(3);
		close(pipefd[1]); //íróvég
		printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
		read(pipefd[0], sz, sizeof(sz)); //pipefd[0] olvasóvég
		printf("Gyerek olvassa az uzenetet %s\n",sz);
		close(pipefd[0]);
	}
	
	//parent
	else{
		printf("Szulo indul!\n");
		close(pipefd[0]);
		write(pipefd[1], "Hajra Fradi!", 13);
		close(pipefd[1]);
		printf("A szulo vegzett a beirassal!\n");
		fflush(NULL);
		wait();
		printf("A szulo befejezte!\n");
		
	}
	
	return 0;
	
}