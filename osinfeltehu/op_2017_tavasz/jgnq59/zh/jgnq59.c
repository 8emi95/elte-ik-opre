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
	int pipefd[2];
	char lakcim[200];
	pid_t utas;
	
	if (pipe(pipefd) == -1) {
		perror("Hiba a pipe nyitaskor");
		exit(EXIT_FAILURE);
	}
	
	utas = fork();
	if (utas == -1) {
		perror("Utas fork hiba");
		exit(EXIT_FAILURE);
	}
	
	if (utas == 0) {
		printf("Gyerek kuldi a lakcimet: %s! \n", argv[1]);
		write(pipefd[1], argv[1], sizeof(argv[1]));
		close(pipefd[1]);
		fflush(NULL);
	} else {
		read(pipefd[0], lakcim, 200);
		printf("Szulo olvasta a lakcimet: %s", lakcim);
		close(pipefd[0]);
		int tmp;
		waitpid(utas, &tmp, 0);
	}
	return 0;
}