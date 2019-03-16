#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(){
//pipefd[0] olvasófej
//pipefd[1] írófej
	int pipefd[2];
	int pipefd2[2];
	pid_t pid;
	pid_t pid2;
	
	int status;
	
	int pipefd3[2];
	
	char sz[100];
	char sz2[100];
	
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
		srand(time(NULL)); //the starting value of random number generation
		int r=(rand()%4)+1;
		printf("Random number %i\n",r);
		char c;
		c=(char) r;
		
		
		printf ( "Child : Parents PID: %d\n", getppid());
		//close(pipefd[1]);
		printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
        read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
        printf("Gyerek olvasta uzenet: %s",sz);
	    printf("\n");
		close(pipefd[0]);
		sleep(3);
		
		write(pipefd[1], "Hajra Vasas!",13);
		close(pipefd[1]);
		
		//válaszol
		close(pipefd3[0]);
		printf("A gyerek valaszol!\n");
		write(pipefd3[1], r,sizeof(r));
        close(pipefd3[1]);
		
	}
	else{
	//második gyerek
		pid2 = fork();
		if(pid2==0){
			printf ( "Child : Parents PID2: %d\n", getppid());
			close(pipefd2[1]);
			printf("Gyerek2 elkezdi olvasni a csobol az adatokat!\n");
			read(pipefd2[0],sz,sizeof(sz)); // reading max 100 chars
			printf("Gyerek2 olvasta uzenet: %s",sz);
			printf("\n");
			close(pipefd2[0]);
			sleep(3);
		}
		
		else{
			//szülõfolyamat
			//wait();
			
			printf("Szulo indul!\n");
			close(pipefd[0]); //olvasófej
			write(pipefd[1], "Kerdes", 6);
			close(pipefd[1]);
			close(pipefd2[0]); //olvasófej
			write(pipefd2[1], "Kerdes", 6);
			close(pipefd2[1]);
			
			
			wait();
			//waitpid(pid, &status, 0);
			//waitpid(pid2, &status, 0);
			
			//fogad
			close(pipefd3[1]);
			printf("A szulo olvassa a valaszt!\n");
			read(pipefd3[0],sz,sizeof(sz));
			printf("A szulo megkapta az uzenetet: %s", sz);
			printf("\n");
			close(pipefd3[0]);
			
			waitpid(pid, &status, 0);
			waitpid(pid2, &status, 0);
		}
		
	}
	
	exit(EXIT_SUCCESS);
}