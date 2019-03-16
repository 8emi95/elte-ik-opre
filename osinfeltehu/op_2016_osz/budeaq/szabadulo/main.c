#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <stdbool.h>

/*
struct Player{
 char name[100];
 int exam;
 int index; 
};
*/

int random_num(int max);
void handler();

int main(int argc,char *argv[]){
	system("clear");
	//KAPTUNK ARGUMENTUMOT VAGY SEM
	if(argc < 2){
		printf("[HIBA] Nem kaptam argumentumot!\n");
		exit(1);
	}
	
	//CSÕ LÉTREHOZÁSA
	int mypipe[2];
	if(pipe(mypipe) == -1){
      printf("[HIBA] Nem tudtam letrehozni a csovet!\n");
      exit(1);
    }
	
	//GYEREK LÉTREHOZÁSA
	pid_t child = fork();
    if (child < 0){
		printf("[HIBA] Nem szuletett meg a gyerek!\n");
		exit(1);
	}
	if(child > 0){   //PARENT
		//printf("Letrejottem\n");

		//FELADAT;
		char quest[100] = "[FELADAT] Old meg a vizsgat!";
		printf("[TRAINER] Most kuldom a vizsgazonak a feladatot. \n");
		write(mypipe[1], &quest, sizeof(quest));
		sleep(1);
		
		//NÉV BEOLVASASA
		char name[100] = " ";
		int i;
	    for (i=0; i<sizeof(argv[1]); i++){
		   name[i] = argv[1][i];
        }
		
		//NEV ELKULDESE
		printf("[TRAINER] Most kuldom a jatekonak a nevet. \n");
		write(mypipe[1], name, sizeof(name));
		sleep(1);
		
		//INDEX
		int index = random_num(100); 
		printf("[TRAINER] Most kuldom a jatekosnak a sorszamat.\n");
		write(mypipe[1], &index, sizeof(index));
		sleep(1);
		
		signal(SIGUSR1, handler);
		pause();
		
		//OLVASÁS A CSÕBÕL ÉS EREDMÉNY KIIÓÍRÁSA
		bool ready;
		read(mypipe[0], &ready, sizeof(ready));
		if (ready){
			printf("[TRAINER] A vizsgazonak sikerult megoldani a feladatot.\n");
		}
		else{
			printf("[TRAINER] A vizsgazonak nem sikerult megoldani a feladatot.\n");
		}
	}else{          //CHILD
	    char quest[100];
		read(mypipe[0], &quest, sizeof(quest));
		printf("[VIZSGAZO] Kiolvastam a feladatom: %s\n", quest);
		sleep(1);
		
		char name[100];
		read(mypipe[0], &name, sizeof(name));
		printf("[VIZSGAZO] Kiolvastam a nevem: %s\n", name);
		sleep(1);
		
		int index;
		read(mypipe[0], &index, sizeof(index));
		printf("[VIZSGAZO] Kiolvastam a sorszamomat: %d\n", index);
		sleep(1);
		
		int check = random_num(10);
		bool ready;
		if(check < 6){
			ready = true;
			printf("[VIZSGAZO] A feladatomat sikeressen elvegeztem!\n");
		}
		else{
			ready = false;
			printf("[VIZSGAZO] A feladatomat sikeressen elvegeztem!\n");
		}
		
		write(mypipe[1], &ready, sizeof(ready));
		
		printf("[VIZSGAZO] Jelzest kuldok amikor keszvagyok.\n");
		sleep(1);
		kill(getppid(), SIGUSR1);	
	}
	
	return 0;
}

//RANDOM GENERÁLÁS
int random_num(int max){
	srand (time(NULL));
	return rand() % max;
}

//SIGNAL HANDLER
void handler(int sign){
	if (sign == SIGUSR1){
		signal(SIGUSR1, SIG_DFL);
        printf("[SIGNAL] Signal erkezett!\n");
    }
	/*else if (sign == SIGUSR2){
		signal(SIGUSR2, SIG_DFL);
		printf("[SIGNAL] Signal erkezett!\n");
	}*/
	else{
		printf("[SIGNAL] %d", sign);
	}
}