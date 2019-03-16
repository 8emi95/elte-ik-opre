#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>

struct jelentes {
	int sikeresseg;
	int n;
	char nemjottek[100][100];
};
struct rendezveny {
	char helyszin[100];
	int n;
	char resztvevok[100][100];
};

void handler(int signumber) {
}

int main() {
	int pipefd[2][2], i;
	char s[100];
	pid_t child;

	struct rendezveny rend[2] = {{
		"Zalaegerszeg",
		4,
		{"valaki", "senki", "akarki", "barki"}
	}, {
		"Budapest",
		1,
		{"senki"}
	}};

	struct sigaction sigact;
	struct jelentes jel;

	signal(SIGUSR1, handler);
	int j;
	for(j = 0; j < sizeof(rend) / sizeof(struct rendezveny); j++) {
	if ((pipe(pipefd[0]) < 0) || (pipe(pipefd[1]) < 0)) {
		perror("Pipe error");
		exit(1);
	}


	child = fork();

	if (child < 0) {
		perror("The fork calling was unsuccessful.");
		exit(1);
	}

	if (child > 0) { // parent
		char buf[100];
		close(pipefd[0][0]);
		close(pipefd[1][1]);
		write(pipefd[0][1], rend[j].helyszin, strlen(rend[j].helyszin));
		pause();
		sprintf(buf, "%d", rend[j].n);
		write(pipefd[0][1], buf, sizeof(buf));
		for(i = 0; i < sizeof(rend[j].resztvevok); i++) {
			write(pipefd[0][1], rend[j].resztvevok[i], sizeof(rend[j].resztvevok[i]));
		}

		read(pipefd[1][0], buf, sizeof(buf));
		jel.sikeresseg = atoi(buf);
		read(pipefd[1][0], buf, sizeof(buf));
		jel.n = atoi(buf);
		for(i = 0; i < jel.n; i++) {
			read(pipefd[1][0], jel.nemjottek[i], sizeof(jel.nemjottek[i]));
		}
		//printf("sik: %d\n", jel.sikeresseg);

		wait(NULL);
		close(pipefd[0][1]);
		close(pipefd[1][0]);
	} else { // child
		int ppid = getppid(), n;
		close(pipefd[0][1]);
		close(pipefd[1][0]);
		int nbytes = read(pipefd[0][0], s, sizeof(s));
		// printf("Olvasott uzi: %s %d\n", s, nbytes);
		srand(time(NULL));
		int r = rand() % 5;
		sleep(r);
		kill(ppid, SIGUSR1);
		nbytes = read(pipefd[0][0], s, sizeof(s));
		n = atoi(s);
		char resztvevok[n][100];
		printf("Resztvevok:\n");
		for(i = 0; i < n; i++) {
			read(pipefd[0][0], s, sizeof(s));
			printf("%s\n", s);
			strcpy(resztvevok[i], s);
		}
		srand(time(NULL));
		jel.sikeresseg = rand() % 100;
		jel.n = 0;
		for(i = 0; i < n; i++) {
			srand(time(NULL));
			if((rand() % 100) > 90) {
				strcpy(jel.nemjottek[jel.n], resztvevok[i]);
				jel.n = jel.n + 1;
			}
		}
		char buf[100];
		sprintf(buf, "%d", jel.sikeresseg);
		write(pipefd[1][1], buf, sizeof(buf));
		sprintf(buf, "%d", jel.n);
		write(pipefd[1][1], buf, sizeof(buf));
		for(i = 0; i < jel.n; i++) {
			write(pipefd[1][1], jel.nemjottek[i], sizeof(jel.nemjottek[i]));
		}
		close(pipefd[0][0]);
		close(pipefd[1][1]);
	}
	}
}
