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
	
			struct tm *ptr_time;
	char buffer[15];

	//Get the current time
	time(&timeInMsg);

	//localtime returns a pointer to a tm structure
	ptr_time = localtime(&timeInMsg);
		
		strftime( buffer,15, "%Y:%m:%d",ptr_time);
		//printf("%s\n", buffer);
	
	printf("Kartal kuld Jelentkezesi idot: %s", ctime(&jelentkezesIdo));
	close(fdOzd[0]);
	write(fdOzd[1], &buffer, 15);
	close(fdOzd[1]);
	
	close(fdKomlod[0]);
	write(fdKomlod[1], &buffer, 15);
	close(fdKomlod[1]);
}



void kiir(char fioknev[]){
	printf("%s, nyitasi ido: %s\n", fioknev, ctime(&timeInMsg));
}


//pecsetelo
void ozd() {
	char b[15];
	kill(getppid(), SIGUSR1);
	printf("ozdi fiokiroda vagyok.\n");
	sleep(3);	
	close(fdOzd[1]);
    read(fdOzd[0], &b, 15); 
    close(fdOzd[0]);
	printf("Ozd, nyitasi ido: %s\n",  b);

	
	/*struct tm *ptr_time;
char buffer[15];

//Get the current time
time(&timeInMsg);

//localtime returns a pointer to a tm structure
ptr_time = localtime(&timeInMsg);
	
	strftime( buffer,15, "%Y:%m:%d",ptr_time);
	printf("%s\n", buffer);*/
}

//ellenor
void komlod() {
	char b[15];
	kill(getppid(), SIGUSR1);
	printf("komlodi fiokiroda vagyok.\n");
 	sleep(3);	
	close(fdKomlod[1]);
    read(fdKomlod[0], &b, 15); 
    close(fdKomlod[0]);
	printf("Komlod, nyitasi ido: %s\n",  b);
	
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