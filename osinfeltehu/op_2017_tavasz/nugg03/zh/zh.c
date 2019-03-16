#include <stdio.h>
#include <unistd.h> //fork
#include <sys/wait.h> //waitpid
#include <stdlib.h> //exit
#include <string.h> //strlen
#include <sys/time.h> //time
#include <signal.h> //sigval
	
void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main(int argc, char* argv[]){
	//pid_t child_a = getpid();
	pid_t child_b;
	int pipefd[2];
	int status;
	signal(SIGUSR1,handler);
	
	if (pipe(pipefd) == -1) {
        perror("Hiba a pipe nyitaskor!");
        exit(1);
	}
	
	child_b = fork(); 
	if (child_b == -1)
	{
		perror("Fork hiba");
		exit(EXIT_FAILURE);
	}

	if(child_b == 0) { // Child
		sleep(3);
		printf("Utas megérkezett!\n");
		char address[1000];
		strcpy(address,argv[1]);
		close(pipefd[0]);
		write(pipefd[1],address,sizeof(address));
		close(pipefd[1]);  
		printf("Az utas bemondta a címét a csőbe!\n");
		kill(getppid(),SIGUSR1);
		printf("Az utas végzett!\n");
	}
	else { // Parent
		printf("Központ elindult!\n");
		char address[1000];
		pause();
		close(pipefd[1]);  
	    printf("A központ elkezdi olvasni az adatokat a csőböl!\n");
        read(pipefd[0],address,sizeof(address)); 
		printf("Az utastól kapott cím: %s \n",address);
		close(pipefd[0]);   
		wait(&status);
		printf("A kozpont vegzett\n");
	}
	
	return 0;
}