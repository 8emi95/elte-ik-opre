#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //open
#include <sys/stat.h>
#include <fcntl.h> //open,creat
#include <string.h>
#include <time.h>
#include <unistd.h> // for pipe()
#include <signal.h>

int main(int argc,char ** argv){
	if(argc!=2){
		printf("A program egy parametert var: jaratok szama\n");
		exit(1);
	}
	
	int max=atoi(argv[1]);
	printf("Szerelvenyek szama: %i\n", max);
	
	srand ( time(NULL) );
	int pipepw[2];
	int pipecw[2];
	pid_t pid;
	
	if (pipe(pipepw) == -1){
		perror("Hiba a pipe megnyitasakor!");
		exit(1);
	}
	if (pipe(pipecw) == -1){
		perror("Hiba a pipe megnyitasakor!");
		exit(1);
	}
	
	pid = fork();
	if (pid == -1) {
		perror("Fork hiba");
		exit(1);
	}
	if (pid > 0){	// Szulo
		close(pipepw[0]);
		close(pipecw[1]);
		// Hanyadik szerelveny szamitasa
		int metroToday = (rand()%max)+1;
		printf("BMF: A mai nap te a(z) %i. metro vagy.\n", metroToday);
		write(pipepw[1],&metroToday,sizeof(metroToday));
		
		// Szerelveny uzenete
		char* msg;
		read(pipecw[0],&msg,sizeof(msg));
		printf("BMF: %s",msg);
		
		close(pipepw[1]);
		close(pipecw[0]);
	} 
	else {	// Gyerek
		close(pipepw[1]);
		close(pipecw[0]);
		// Hanyadik szerelveny ma
		int metroToday = 0;
		read(pipepw[0],&metroToday,sizeof(metroToday));
		int metroLeft = max-metroToday;
		printf("Metro: A mai nap én vagyok a(z) %i. metro. Utanam meg %i lesz.\n", metroToday, metroLeft);
		int succes = rand()%10;
		char* msg;
		if(succes==0){
			printf("Metro: Jajj ne, egy bananhej!!!\n");
			printf("Metro: Nem teljesitettem sikeresen az utat.\n");
			msg = "A metro nem teljesitette az utat, mert elcsuszott egy bananhejon.\n";
			
		} else {
			printf("Metro: Sikeresen teljesitettem az utat.\n");
			msg = "A metro sikeresen teljesitette az utat, nem csuszott el bananhejon.\n";
		}
		write(pipecw[1],&msg,sizeof(msg));
		close(pipepw[0]);
		close(pipecw[1]);
	}
	
	return 0;
}