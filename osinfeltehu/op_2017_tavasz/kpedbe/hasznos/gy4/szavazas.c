#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>

int szamlalo = 0;
void handler(int signal) {
	szamlalo++;
}

int fd[2];


int main(int argc, char ** argv) {
	int szavazok_szama;
	if(argc != 2) {
		printf("Hat ez kellemetlen...\n nem irt be a kedves felhasznalo argumentumot...\n igy most kenytelen vagyok elszallni :( \n");
		exit(1);
	}
	szavazok_szama = atoi(argv[1]);
	
	signal(SIGUSR1,handler);
	if(pipe(fd) < 0) {
		printf("Hat ez kellemetlen...\n nem sikerult csovelni...\n igy most kenytelen vagyok elszallni :( \n");
		exit(1);
	}
	
	pid_t ellenor = fork();
	if(ellenor > 0) {
		pid_t pecsetelo = fork();
		if(pecsetelo > 0) {
			while(szamlalo != 2) {
				usleep(100);
			}
			printf("SZULO elindult!\n");
			int i = 0;
			srand(time(NULL));
			for(i = 0; i < szavazok_szama; i++) {
				int azon = rand();
				write(fd[1],&azon,sizeof(azon));
			}
			close(fd[1]);
			wait(NULL);
			wait(NULL);
		} else {
			kill(getppid(), SIGUSR1);
			printf("PECSETELO elindult es OLT xD\n");
			close(fd[1]);
			printf("PECSETELO  befejezi a kuldeteset\n");
		}
	} else {
		kill(getppid(), SIGUSR1);
		printf("ELLENOR elindult es OLT xD\n");
		int azon;
		close(fd[1]);
		while(read(fd[0],&azon,sizeof(azon)) > 0) {
			printf("%i\n", azon);
		}
		close(fd[0]);
		printf("ELLENOR befejezi a kuldeteset\n");
	}
	
}