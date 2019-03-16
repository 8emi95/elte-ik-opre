//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int szavazok;
int szamlalo;
int pipeFd[2];

void handler(int signalNumber) {
	//printf("Signal sent: %d\n", signalNumber );
	szamlalo++;
}

void elnok() {
	printf("Elnok vagyok !!\n");
}

void pecsetelo() {
	kill(getppid(), SIGUSR1);
	printf("Pecsetelo vagyok !!\n");
}

void ellenorzo() {
	kill(getppid(), SIGUSR1);
	printf("Ellenor vagyok !!\n");
}

int main(int argc,char** argv){
	signal(SIGUSR1, handler);
	szamlalo = 0;
	szavazok = atoi(argv[1]);
	printf("szavazok %d", szavazok);

	pid_t ellen = fork();
	if (ellen > 0) {
		pid_t pecset = fork();
		if (pecset > 0) {
			while(szamlalo != 2) {;}
			elnok();
			wait(NULL);
			wait(NULL);
		} else {
			pecsetelo();
		}
	}
	else {
		ellenorzo();
	}



	return 0;
}