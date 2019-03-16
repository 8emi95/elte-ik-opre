#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>

int fd[2];
int szavazokSzama;

void elnok() {
	printf("Elnok vagyok - mindenki kesz\n");
	
	int i;
	int azonosito;
	
	printf("Elnok vagyok - azonositokat gyartottam\n");
	close(fd[0]);
	
	for (i = 0; i < szavazokSzama; i++) {
		azonosito = rand() % 10000;
		write(fd[1], &azonosito, sizeof(int));
		printf("%i ,\n", azonosito);
	}
	
	printf("Elnok vagyok - beirtam az azonositokat a csobe\n");
	
	close(fd[1]);
}

void ellenorzo() {
	kill(getppid(), SIGUSR1);
	printf("\nEllenorzo vagyok\n");
	
	int azonosito;
	
	close(fd[1]);
	
	while (read(fd[0], &azonosito, sizeof(int))) {
		printf("Ellenor: %i\n", azonosito);
		fflush(stdout);
	}
	
	close(fd[0]);
	
	printf("Ellenor vagyok - csobol olvastam/irtam");
}

void pecset() {
	kill(getppid(), SIGUSR1);
	printf("\nPecsetelo vagyok\n");
}

volatile szamlalo = 0;

void handler(int sn) {
	szamlalo++;
}

int main(int argc, char **argv) {
	srand(time(NULL));
	signal(SIGUSR1, handler);
	
	if (argc!=2) {
		perror("Argumentum hiba!\n");
		exit(1);
	}
	
	szavazokSzama = atoi(argv[1]);
	
	if (pipe(fd) < 0){
		perror("Csovezetek hiba\n");
		exit;
	}
	
	pid_t ellen = fork();
	if (ellen > 0) {
		pid_t pecsetelo = fork();
		if (pecsetelo > 0) {
			//Szulo biztosan
			while (szamlalo != 2){;} //pause helyett
			
			elnok();
			
			wait(NULL);
			wait(NULL);
		} else {
			// Pecsetelo
			pecset();
		}
	} else {
		// Ellenorzo
		ellenorzo();
	}
	
	return 0;
}