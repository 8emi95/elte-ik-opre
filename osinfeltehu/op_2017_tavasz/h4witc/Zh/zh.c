#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char * argv[])
{
	int pipefd[2];
	pid_t pid;
	char sz[sizeof argv[1]];
	if(argc == 1){
		perror("nincs argumentum megadva");
		exit(-1);
	}
	if(pipe(pipefd) == -1){
		perror("hiba nyitaskor");
		exit(-1);
	}
	pid= fork();
	if(pid == -1){
		perror("fork hiba");
		exit(-1);
	}
	if(pid == 0){
		close(pipefd[0]);
		printf("utas bemondja a cimet: %s \n", argv[1]);
		write(pipefd[1],argv[1],sizeof(argv[1]));
		close(pipefd[1]);
		printf("Az utas bemondta a cimet. \n");
		fflush(NULL);
		wait();
		//printf("kozpont vege \n");
	
	}else{
		close(pipefd[1]);
		read(pipefd[0],sz,sizeof(sz));
		printf("kozpont fogadta a cimet ami: %s \n", sz);
		close(pipefd[0]);
	}
	exit(1);	
	
}