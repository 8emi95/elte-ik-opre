#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>

struct Versenyzo {
	char* nev;
	char* feladat;
	int sorszam; 
	int sikeres; // 0 - nem, 1 -igen
};

void gyerek();
void vezeto();

void handler(int signumber) {
	printf("Megjott a jelzes!\n");
}

int main(int argc,char *argv[]){
	srand ( time(NULL) );
	signal(SIGUSR1,handler);
	
	int cso1[2]; //egy cso, amibe ír a szülő, olvas a gyerek
	int cso2[2]; //egy cso, amibe ír a gyerek, olvas a szulo
	
	pid_t pid;
		
	if(pipe(cso1) == -1){
		perror("hibas cso nyitas");
		exit(EXIT_FAILURE);
	}
	
	if(pipe(cso2) == -1){
		perror("hibas cso nyitas");
		exit(EXIT_FAILURE);
	}
	
	pid = fork();
	if(pid == -1){
		perror("Hiba a gyerekfolyamat letrehozasakor");
		exit(EXIT_FAILURE);	
	}
	if(pid > 0){  //szulo
		int i = 1;
		while (i <= argc){
			pid_t pid2;
			pid2 = fork();
			if(pid2 == -1){
				perror("Hiba a gyerekfolyamat2 letrehozasakor");
				exit(EXIT_FAILURE);	
			}
			if(pid2 > 0){
				printf("SZULO -- fut\n");
				
				struct Versenyzo versenyzo;
			
				close(cso1[0]);
				close(cso2[1]);
				
				
				char v_nev[1024];
				strcpy(v_nev, argv[i]);
				int v_sorszam = i;
				char* feladat = "feladat1";
				char v_feladat[1024];
				strcpy(v_feladat, feladat);
				
				
				write(cso1[1], v_nev, sizeof(v_nev));
				write(cso1[1], &v_sorszam, sizeof(v_sorszam));
				write(cso1[1], 	v_feladat, sizeof(v_feladat));
				fflush(NULL);
				
				pause();
				
				int sikeres;
				read(cso2[0],&sikeres, sizeof(&sikeres));
				
				if (sikeres == 0){
					printf("SZULO -- a gyerek nem vegezte el a feladatot\n");
				}
				else{
					printf("SZULO -- a gyerek elvegezte a feladatot\n");
				}
				
				printf("SZULO -- befejezett egy gyereket\n");
			}
			else{
				printf("GYEREK -- fut\n");
				int szulopidszam = getppid();
				
				close(cso1[1]);
				close(cso2[0]);
				
				char v_nev[1024];
				int v_sorszam;
				char v_feladat[1024];
				
				
				read(cso1[0], v_nev, sizeof(v_nev));
				read(cso1[0], &v_sorszam, sizeof(v_sorszam));
				read(cso1[0], v_feladat, sizeof(v_feladat));
				
				printf("GYEREK -- megkapott versenyzo neve: %s\n", v_nev);
				printf("GYEREK -- megkapott versenyzo sorszama: %d\n", v_sorszam);
				printf("GYEREK -- megkapott versenyzo feladata: %s\n", v_feladat);
					
				
				int random = rand() % 3;
				
				
				write(cso2[1], &random, sizeof(random));
				fflush(NULL);
				
				kill(szulopidszam, SIGUSR1);
				
				printf("GYEREK -- leallt\n");
				sleep(3);
			}
			i = i + 1;
		}
		
		close(cso1[0]);
		close(cso2[1]);
		close(cso1[1]);
		close(cso2[0]);
		
		int status;
		waitpid(pid, &status, 0);
		printf("SZULO -- leallt\n");
	}
	else{  //gyerek
	printf("VEZETO -- fut\n");
		/*printf("GYEREK -- fut\n");
		int szulopidszam = getppid();
		
		close(cso1[1]);
		close(cso2[0]);
		
		char v_nev[1024];
		int v_sorszam;
		char v_feladat[1024];
		
		
		read(cso1[0], v_nev, sizeof(v_nev));
		read(cso1[0], &v_sorszam, sizeof(v_sorszam));
		read(cso1[0], v_feladat, sizeof(v_feladat));
		
		printf("GYEREK -- megkapott versenyzo neve: %s\n", v_nev);
		printf("GYEREK -- megkapott versenyzo sorszama: %d\n", v_sorszam);
		printf("GYEREK -- megkapott versenyzo feladata: %s\n", v_feladat);
			
		
		int random = rand() % 3;
		
		
		write(cso2[1], &random, sizeof(random));
		fflush(NULL);
		
		kill(szulopidszam, SIGUSR1);
		
		close(cso1[0]);
		close(cso2[1]);
		
		printf("GYEREK -- leallt\n");*/
	}
	return 0;
}
