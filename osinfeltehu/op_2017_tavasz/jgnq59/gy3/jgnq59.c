#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <wait.h> 

int main(int argc, char *argv[]) {
	int pipefd[1];
	char lakcim[200];
	
	if (pipe(pipefd) == -1) {
		perror("Hiba a pipe nyitaskor");
		exit(EXIT_FAILURE);
	}
	
	pid_t utas;
	//pid_t kocsi;
	
	utas = fork();
	if (utas == -1) {
		perror("Utas fork hiba");
		exit(EXIT_FAILURE);
	}
	
	/*kocsi = fork();
	if (kocsi == -1) {
		perror("Auto fork hiba");
		exit(EXIT_FAILURE);
	}*/
	
	if (utas == 0) {
		printf("Gyerek kuldi a lakcimet!");
		write(pipefd[0], argv[0], sizeof(argv[0]));
	} else {
		sleep(1);
		read(pipefd[0], lakcim, 200);
		printf("Szulo olvasta a lakcimet: %s", lakcim);
	}
	
}