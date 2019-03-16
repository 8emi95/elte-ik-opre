#include <stdlib.h>
#include <stdio.h>
#include <signal.h>		//jelzések
#include <sys/types.h>	//soksok típus
#include <string.h>		//string művleetek
#include <unistd.h> 	//csövezés

//Esemény lekezelése
void handler(int signumber){
	printf("Signal handled\n");
}

int main(int argc,char ** argv) {

  struct sigaction sigact; //struktúra a jelzés (signál) küldéséhez
  sigact.sa_handler=handler; //az eseménykezelő hozzárendelése
  sigemptyset(&sigact.sa_mask); //azt mondjuk, hogy senkit nem kezelünk le
  sigact.sa_flags=0; //nem lesz különleges tevékenység
  sigaction(SIGTERM,&sigact,NULL);		// létrehozzuk magukat a jelzéseket

	
	int i;
	int n = 2;
	int pipefd[n];
	char buffer[1024];
	
	if (pipe(pipefd) == -1)					//cső megnyitása
    {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }
	
	pid_t pids[n];	//tömb n gyerek tárolására
	// n gyerek indítása
	for (i = 0; i < n; i++) {
	  if ((pids[i] = fork()) < 0) {
		perror("fork");			//hibakezelés
		abort();
	  } else if (pids[i] == 0) {
		//Gyerek feladatai
		char c[1024];
		switch(i){
			case 0 : strcpy(c,"Ozd"); break;
			case 1 : strcpy(c,"Komlo"); break;
		}
		kill(getppid(),SIGTERM);	//Esemény(=signal) létrehozása, amit majd a szüő kezel le, pid = aktualis folyamat prcessz id-ja, ppid - parent-||-
		read(pipefd[0],buffer,sizeof(buffer));
		printf("%s a kapott adatot: %s\n",c, buffer);
		exit(0);
	  }
	}
	//PARENT

	sigset_t sigset;					//ez a rész csinálja a kezelést
    sigfillset(&sigset);				//ez a sor kiúrití a várt jelzéseket
    sigdelset(&sigset,SIGTERM);			//itt beálltjuk, hogy SIGTERM-et várunk
    sigsuspend(&sigset);				//várunk amig van ilyen jelzés életben
	strcpy(buffer,"IDE A DATUM KENE, CSAK FARADT VAGYOK");
	for(i=0;i<n;i++){
		write(pipefd[1], buffer ,sizeof(buffer));		//n-szer ráírjuk az adatot a csőre
	}
	
	for(i=0;i<n;i++){
		wait();					//megvárjuk, míg mind az n gyerek befejeződik
	}
	printf("Szulo vege\n");
	return 0;
}