#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <signal.h>

int main(int argc, char *argv[]){

	int pipefd[2];
	pid_t pid;
	
	char sz[100];
	
	if (pipe(pipefd) == -1){
		perror("Error making pipe! \n");
		exit(EXIT_FAILURE);
	}
	
	pid = fork();
	if (pid == -1){
		printf("Fork ERROR! \n");
		exit(EXIT_FAILURE);
	}
	
	//char t[1024] = {'a', 'b', 's'};
	
	char c[1024];
	strcpy(c, "safsdgfa");
	strcat(c, ", ghjsad");
	strcat(c, ", hasdger");
	char t[1024];
	// parent
	if ( pid != 0){
		printf("Parent starting! \n");
		close(pipefd[0]);
		//write(pipefd[1], "sending from parent", 20);
		write(pipefd[1], c, sizeof(c));
		close(pipefd[1]);
		fflush(NULL);
		wait();
		/*
		fflush(NULL);
		read(pipefd[1], sz, sizeof(sz));
		printf("Parent read: %s \n", sz);
		*/
		printf("Parent done writing! \n");
		
	}
	//child
	else {
		sleep(2);
		//kill(getppid(), SIGTERM);
		printf("Child starting \n");
		close(pipefd[1]);
		printf("Reading from pipe! \n");
		
		sleep(2);
		read(pipefd[0], t, sizeof(t));
		printf("Child could read: %s \n", t);
		
		
		
		/*int k;
		for (k = 0; k < strlen(t) ; ++k) {
			printf("%s", t[k]);
			if (k % 5 == 0){
				printf("\n");
			}
			
		}*/
		
		sleep(2);
		/*
		fflush(NULL);
		write(pipefd[0], "hello", 6);
		sleep(2);
		*/
		close(pipefd[0]);
	}
}