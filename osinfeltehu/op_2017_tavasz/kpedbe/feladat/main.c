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
	
	pid_t utas = fork(); //elind�tom az els� gyereket
	if(utas > 0) { //k�zpont �ga
		pid_t autos = fork(); //elind�tom a m�sodik gyereket (autot)
		if(autos > 0) { //m�sodik gyerek sz�l�je
			while(szamlalo != 2) {
				usleep(100); //v�r
			}
			printf("SZULO elindult!\n");
			//close(fd[1]);
			read(fd[0],&lakcim,sizeof(lakcim));
			close(fd[0]);
			printf("%s\n",lakcim);
			wait(NULL); 
			wait(NULL); 
		} else { //autos �ga
			kill(getppid(), SIGUSR1); // ind�tsa a jelz�st, hogy k�szen �ll az autos gyerek
			printf("Autos elindult\n");
		}
	} else { //utas �ga
		kill(getppid(), SIGUSR1); //k�ldje a jelz�st, hogy az utas k�szen �ll
		printf("UTAS elindult\n");
		close(fd[0]);//olvas� z�r
		write(fd[1],&lakcim,sizeof(lakcim));
		close(fd[1]); //ir� z�r
	}
	
}

// _sleep(int) f�ggv�ny miliszekundumban m�r.