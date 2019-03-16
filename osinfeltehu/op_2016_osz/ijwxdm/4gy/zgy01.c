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
int fd[2];
int szamlalo;
time_t jelentkezesIdo;
time_t timeInMsg;

//elnok
	void kartal() {
//void kartal(n) {
  //  int i, azon;
	printf("Kartal (Elnok) vagyok.\n\n");
	sleep(3);
   /* close(fd[0]);
    for (i=0; i<n; ++i) {
        azon=rand()%10000;
        write(fd[1], &azon, sizeof(int));
    }
    close(fd[1]);*/
	printf("Kozpont valaszt kuld!\n");
	time_t ltime;
	
	jelentkezesIdo = time(&ltime);
	printf("Kartal kuld Jelentkezesi idot: %s", ctime(&jelentkezesIdo));
	close(fd[0]);
	write(fd[1], &jelentkezesIdo, sizeof(time_t));
   // write(fd[1], ctime(&jelentkezesIdo), 100);
   // write(fd[1], "valami", 100);
	close(fd[1]);
	
}

void olvas(){
	sleep(3);	
	//time_t timeInMsg= time(&ltime);
	//time_t timeInMsg;
	//char timeInMsg[100];
	close(fd[1]);
    read(fd[0], &timeInMsg, sizeof(timeInMsg)); 
  //  printf("fioknev, nyitasi ido: %s\n",  ctime(&timeInMsg));
   //  read(fd[0], &timeInMsg, 100); 
   //  printf("fioknev, nyitasi ido: %s\n",  timeInMsg);
    close(fd[0]);
}

void kiir(char fioknev[]){
	printf("%s, nyitasi ido: %s\n", fioknev, ctime(&timeInMsg));
}


//pecsetelo
void ozd() {
	kill(getppid(), SIGUSR1);
	printf("ozdi fiokiroda vagyok.\n");
	
	
}

//ellenor
void komlod() {
	kill(getppid(), SIGUSR1);
	printf("komlodi fiokiroda vagyok.\n");
 /*   int azon;
    close(fd[1]);
    while(read(fd[0], &azon, sizeof(int))) {
           printf("ELLENOR: vallaszto erkezett: azonosito: %d\n", azon);
    }
    close(fd[0]);*/
	
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
	
    if (pipe(fd)<0) {
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
			olvas();
			kiir("ozd");
		}
	}
	else {
		komlod();
		olvas();
		kiir("komlod");
	}
}