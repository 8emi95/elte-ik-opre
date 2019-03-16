#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

int szamlalo = 0;
void handler(int signal) {
	szamlalo++;
}

int fd[2];


int main(int argc, char ** argv) { 
	char* lakcim;
	if(argc != 2) {
		printf("Nincs lakcim megadva\n");
		exit(1);
	}
	strcpy(lakcim,argv[1]);
	
	signal(SIGUSR1,handler); 
	if(pipe(fd) < 0) {
		printf("Hiba\n");
		exit(1);
	}
	
	pid_t utas = fork(); //elindítom az elsõ gyereket
	if(utas > 0) { //központ ága
		pid_t autos = fork(); //elindítom a második gyereket (autot)
		if(autos > 0) { //második gyerek szülõje
			while(szamlalo != 2) {
				usleep(100); //vár
			}
			printf("SZULO elindult!\n");
			//close(fd[1]);
			read(fd[0],&lakcim,sizeof(lakcim));
			close(fd[0]);
			printf("%s\n",lakcim);
			wait(NULL); 
			wait(NULL); 
		} else { //autos ága
			kill(getppid(), SIGUSR1); // indítsa a jelzést, hogy készen áll az autos gyerek
			printf("Autos elindult\n");
		}
	} else { //utas ága
		kill(getppid(), SIGUSR1); //küldje a jelzést, hogy az utas készen áll
		printf("UTAS elindult\n");
		close(fd[0]);//olvasó zár
		write(fd[1],&lakcim,sizeof(lakcim));
		close(fd[1]); //iró zár
	}
	
}

// _sleep(int) függvény miliszekundumban mér.