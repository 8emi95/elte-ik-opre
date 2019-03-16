#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>

#define MAX_SIZE 1000

void handler(int signal) {
	
}


int fd[2];

int main(int argc, char ** argv) {
	char lakcim[MAX_SIZE];
	if(argc < 2) {
		printf("nincs lakcim\n");
		return 1;
	}
	strcpy(lakcim,argv[1]);
	
	if(pipe(fd) < 0) {
		printf("rossz a cso\n");
		return 1;
	}
	
	
	pid_t utas = fork();
	signal(SIGUSR1,handler);
	if(utas > 0) {
		//szulo
		pause();
		close(fd[1]);
		char utas_lakcime[MAX_SIZE];
		read(fd[0],utas_lakcime,MAX_SIZE);
		printf("KOZPONT: Az utas lakcime: %s\n",utas_lakcime);
		wait();
		
		
	} else if(utas == 0) {
		//utas
		printf("UTAS: kuldom a lakcimet: %s\n",lakcim);
		close(fd[0]);
		write(fd[1],lakcim,MAX_SIZE);
		close(fd[1]);
		kill(getppid(),SIGUSR1);
	}
	
	
	
	return 0;
}