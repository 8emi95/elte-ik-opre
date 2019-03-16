#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <signal.h>

int pipe_1[2], pipe_2[2];

void handler(int signumber) {
	printf("Trener: jatekos kesz\n");
	char message[1024];
	read(pipe_2[0], message, sizeof(message));
	
	printf("Trener: a jatekos azt uzeni: %s\n", message);
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Missing name!\n");
		exit(1);
	}		

	
	pid_t pid;
	if(pipe(pipe_1) == -1) {
		perror("pipe_1");
	}
	if(pipe(pipe_2) == -1) {
		perror("pipe_2");
	}
	
	pid = fork();
	if (pid == -1) {
		perror("Fork");
		exit(-1);
  }
	
	if (pid > 0) {
		signal(SIGUSR1, handler);
		close (pipe_1[0]);
		char message[1024];
		sprintf(message, "%d", pid);
		
		write(pipe_1[1], message, sizeof(message));
		sprintf(message, argv[1]);
		write(pipe_1[1], message, sizeof(message));
		sprintf(message, "Feladat");
		write(pipe_1[1], "Feladat", sizeof("Feladat"));
		close(pipe_1[1]);
		
		int status;
		wait(&status);
	} else if (pid == 0) {
		int num;
		char message[1024];
		
		read(pipe_1[0], message, sizeof(message));
		num = atoi(message);
		printf("Jatekos: sorszamom: %d\n", num);
		read(pipe_1[0], message, sizeof(message));
		printf("Jatekos: Nevem: %s\n", message);
		read(pipe_1[0], message, sizeof(message));
		printf("Jatekos: Feladatom: %s\n", message);
		
		close(pipe_1[0]);
		
		srand(time(NULL));
		
		int result = rand() % 2;
		if (result == 0) {
			sprintf(message, "Nem sikerult");
		} else {
			sprintf(message, "Sikerult");
		}
		printf("Jatekos: Eredmeny: %s\n", message);
		
		write(pipe_2[1], message, sizeof(message));
		kill(getppid(), SIGUSR1);
		
		exit(0);
	}
	
	return 0;
}