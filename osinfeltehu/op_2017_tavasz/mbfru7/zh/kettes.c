#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>

void handler (int signum) {}

int main (int argc, const char* argv[]) {
	if (argc != 2) {
		printf("Lakcím paraméterrel futtassa az alkalmazást!\n");
		exit(1);
	}
	
	int pipefd[2];
	if (pipe(pipefd) == -1) {
		printf("Hiba a csővezeték nyitásakor!\n");
		exit(1);
	}
	
	signal(SIGTERM, handler);
	
	pid_t child = fork();
	if (child < 0) {
		printf("Fork hiba\n");
		exit(1);
	} else if (child > 0) { //parent
		char lakcim[100];
		printf("Központ: Várom az utasok jelentkezését! ...\n");
		pause();
		
		sleep(1);
		printf("Központ: Mondja a címet!\n");
		kill(child, SIGTERM);
		pause();

		sleep(1);
		printf("Központ: Akkor kiolvasom onnan!\n");
		close(pipefd[1]);
		read(pipefd[0], lakcim, sizeof(lakcim));
		close(pipefd[0]);
		sleep(1);
		printf("Központ: Ezt olvastam ki: %s\n", lakcim);
	} else { //child
		sleep(1);
		printf("Utas: Egy taxit szeretnék!\n");
		kill(getppid(), SIGTERM);
		pause();
		
		sleep(1);
		printf("Utas: Inkább elküldöm csővezetéken!\n");
		char* cimem = malloc((sizeof(argv[1]) + 1) * sizeof(char));
		strcpy(cimem, argv[1]);
		close(pipefd[0]);
		write(pipefd[1], cimem, sizeof(cimem));
		close(pipefd[1]);
		fflush(NULL);
		kill(getppid(), SIGTERM);
	}
	
	return 0;
}
