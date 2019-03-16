#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h> //waitpid
#include <unistd.h> //includes fork, pipe()
#include <fcntl.h> // manipulate file descriptor
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h> // for errno, the number of last error
#include <string.h>
#include <signal.h>

//int szavazatokszama;
int fdOzd[2];
int fdKomlod[2];
int szamlalo;
time_t jelentkezesIdo;
time_t timeInMsg;

//elnok
	void kartal() {
	printf("Kartal (Elnok) vagyok.\n\n");
	sleep(3);

	printf("Kozpont valaszt kuld!\n");
	time_t ltime;
	
	jelentkezesIdo = time(&ltime);
	printf("Kartal kuld Jelentkezesi idot: %s", ctime(&jelentkezesIdo));
	close(fdOzd[0]);
	write(fdOzd[1], &jelentkezesIdo, sizeof(time_t));
	close(fdOzd[1]);
	
	close(fdKomlod[0]);
	write(fdKomlod[1], &jelentkezesIdo, sizeof(time_t));
	close(fdKomlod[1]);
}



void kiir(char fioknev[]){
	printf("%s, nyitasi ido: %s\n", fioknev, ctime(&timeInMsg));
}


//pecsetelo
void ozd() {
	kill(getppid(), SIGUSR1);
	printf("ozdi fiokiroda vagyok.\n");
	sleep(3);	
	close(fdOzd[1]);
    read(fdOzd[0], &timeInMsg, sizeof(timeInMsg)); 
    close(fdOzd[0]);
	printf("Ozd, nyitasi ido: %s\n",  ctime(&timeInMsg));
}

//ellenor
void komlod() {
	kill(getppid(), SIGUSR1);
	printf("komlodi fiokiroda vagyok.\n");
 	sleep(3);	
	close(fdKomlod[1]);
    read(fdKomlod[0], &timeInMsg, sizeof(timeInMsg)); 
    close(fdKomlod[0]);
	printf("Komlod, nyitasi ido: %s\n",  ctime(&timeInMsg));
	
}

void handler() {
	szamlalo++;
	printf("Fiokiroda kesz!\n");
}

int main (int argc, char *argv[]) {
	szamlalo = 0;
	signal(SIGUSR1, handler);

/*	if (argc < 2) {
		printf("Hianyzo arg.");
		exit(EXIT_FAILURE);
	}*/

	//szavazatokszama = atoi(argv[1]);
	
    if (pipe(fdOzd)<0) {
		printf("Cso letrehozasa nem sikerult.");
		exit(EXIT_FAILURE);
	}
	
	 if (pipe(fdKomlod)<0) {
		printf("Cso letrehozasa nem sikerult.");
		exit(EXIT_FAILURE);
	}

	pid_t ellen = fork();

	if (ellen > 0) {
		pid_t pecset = fork();

		if (pecset > 0) {
			while(szamlalo<2);
			//elnok(szavazatokszama);
			kartal();
			wait(NULL); wait(NULL);
		}
		
		
		else {
			ozd();
			
		}
	}
	else {
		komlod();
		
		
	}
}