#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(){
	int pipefd[2];
	int pipefd2[2];
	pid_t pid;
	
	char sz[100];
	
	//megnyitjuk az egyik csovet:
	if(pipe(pipefd) == -1) {
		perror("Hiba a csõ megnyitásakor.");
		exit(EXIT_FAILURE);
	}
	
	//megnyitjuk a masik csovet:
	if(pipe(pipefd2) == -1) {
		perror("Hiba a csõ megnyitásakor.");
		exit(EXIT_FAILURE);
	}
	
	//forkkal letrejon a gyerekfolyamat
	pid = fork();
	if(pid == -1) {
		perror("Hiba a forknal.");
		exit(EXIT_FAILURE);
	}
	
	if(pid==0){
	//gyerekfolyamat
		//fogad
		close(pipefd[1]);
		printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
        read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
        printf("Gyerek olvasta uzenet: %s",sz);
	    printf("\n");
		close(pipefd[0]);
		
		//válaszol
		close(pipefd2[0]);
		printf("A gyerek valaszol!\n");
		write(pipefd2[1], "Hajra Vasas!",13);
        close(pipefd2[1]);
		
	}
	else{
	//szülõfolyamat
		//válaszol
		close(pipefd[0]);
		printf("Szulo indul!\n");
		write(pipefd[1], "Hajra Fradi!",13);
        close(pipefd[1]);
		printf("A szulo elkuldte az uzenetet!\n");
		wait();
		
		//fogad
		close(pipefd2[1]);
		printf("A szulo olvassa a valaszt!\n");
		read(pipefd2[0],sz,sizeof(sz));
		printf("A szulo megkapta az uzenetet: %s", sz);
		printf("\n");
		close(pipefd2[0]);
		
	}
	
	exit(EXIT_SUCCESS);
}