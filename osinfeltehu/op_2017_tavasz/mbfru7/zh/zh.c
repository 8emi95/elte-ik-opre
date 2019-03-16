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
		printf("Lakc�m param�terrel futtassa az alkalmaz�st!\n");
		exit(1);
	}
	
	int pipefd[2];
	if (pipe(pipefd) == -1) {
		printf("Hiba a cs�vezet�k nyit�sakor!\n");
		exit(1);
	}
	
	signal(SIGTERM, handler);
	
	int status;
	char lakcim[100];
	pid_t child = fork();
	if (child < 0) {
		printf("Fork hiba\n");
		exit(1);
	} else if (child > 0) { //parent
		printf("K�zpont: V�rom az utasok jelentkez�s�t! ...\n");
		pause();
		
		close(pipefd[1]);
		printf("K�zpont: olvas..\n");
		read(pipefd[0], lakcim, sizeof(lakcim));
		printf("K�zpont: Ezt olvastam ki: %s\n", lakcim);
		close(pipefd[0]);
		
		//waitpid(child, &status, 0);
		printf("Szulo befejezte!");	
	} else { //child
		sleep(3);	// sleeping a few seconds, not necessary
		close(pipefd[0]); //Usually we close unused read end
		write(pipefd[1], "Hajra Fradi!",13);
		close(pipefd[1]); // Closing write descriptor 
		fflush(NULL); 	// flushes all write buffers (not necessary)
		kill(getppid(), SIGTERM);
	}
	
	return 0;
}
