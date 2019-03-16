/************************
**					   **
** Név: Kanyó Kristóf  **
** Neptun: F2QJAO	   **
**                     **
************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <sys/wait.h>

#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <sys/stat.h> 

void handler(int signumber){
    //printf("Signal with number %i has arrived\n",signumber);
}

struct UzenetTartalom {
    int hanyadik;
	int max;
	char megjegyzes[1024];
	int pot;
};

struct Uzenet { 
    long mtype; // Osztályozásra használható
    struct UzenetTartalom tartalom; 
}; 

/* mtype osztályozás jelentései:

1 - Szulo kuld egy uzenetet a gyereknek
2 - A gyerek kuld egy uzenetet a szulonek

*/

int main(int argc, char** argv) {
	if(argc < 2) { // Ellenorzes, hogy megadta-e a szukseges parancssori parametert
		printf("Nincs parancssori parameter!\n");
		exit(-1);
	}
    
	srand(time(NULL));
	
    int status; // Segedvaltozo
	
    signal(SIGUSR1,handler);
	
	key_t kulcs = ftok(argv[0],1); // Kulcs generálása
    int uzenetsor = msgget(kulcs, IPC_CREAT|S_IRUSR|S_IWUSR);
    
    if(uzenetsor < 0) {
        printf("Hiba az uzenetsor letrehozasakor!\n");
        exit(-2);
    }
	
    int child = fork();
    if(child < 0) {
        printf("Hiba a forkolas kozben!\n");
        exit(-1);
    }
    
    if(child > 0) {
        // Szulo - tarsasag
		int osszes = 0;
		int elozo_hibas = 0;
        int max = atoi(argv[1]);
		int hanyadik = 1;
		struct UzenetTartalom tartalom;
		struct Uzenet uzenet;
		
		printf("[SZULO] max: %d\n", max);
		
		while(hanyadik <= max) {
			osszes++;
			tartalom.hanyadik = hanyadik;
			tartalom.max = max;
			
			if(elozo_hibas == 1) {
				tartalom.pot = 1;
			} else {
				tartalom.pot = 0;
			}
			
			uzenet.mtype = 1;
			uzenet.tartalom = tartalom;
			
			printf("[SZULO] Kuldi az uzenetet!\n\n");
			status = msgsnd(uzenetsor, &uzenet, sizeof(struct UzenetTartalom), 0);
			if(status < 0) {
				printf("Hiba az uzenet kuldese kozben!\n");
				exit(-3);
			}
			
			pause();
			printf("[SZULO] Jelzest kap!\n");
			
			status = msgrcv(uzenetsor, &uzenet, sizeof(struct UzenetTartalom), 2, 0);
			if(status < 0) {
				printf("Hiba az uzenet fogadasa kozben!\n");
				exit(-3);
			}
			printf("[SZULO] Megkapta az uzenetet!\n");
			printf("[SZULO] Uzenet: %s\n\n", uzenet.tartalom.megjegyzes);
			
			if(strcmp(uzenet.tartalom.megjegyzes, "Sikeresen beert!") == 0) {
				elozo_hibas = 0;
				char megjegyzes[1024];
				if(hanyadik < max) {
					strcpy(megjegyzes, "Van meg");
				} else {
					strcpy(megjegyzes, "Nincs mar");
				}
				
				uzenet.mtype = 1;
				strcpy(uzenet.tartalom.megjegyzes, megjegyzes);
				
				status = msgsnd(uzenetsor, &uzenet, sizeof(struct UzenetTartalom), 0);
				if(status < 0) {
					printf("Hiba az uzenet kuldese kozben!\n");
					exit(-3);
				}
				
				hanyadik++;
			} else {
				if(elozo_hibas == 1) {
					printf("[SZULO] elkuldi a pot-ot!\n");
					
					char megjegyzes[1024];
					if(hanyadik < max) {
						strcpy(megjegyzes, "Van meg");
					} else {
						strcpy(megjegyzes, "Nincs mar");
					}
					
					uzenet.mtype = 1;
					strcpy(uzenet.tartalom.megjegyzes, megjegyzes);
					
					status = msgsnd(uzenetsor, &uzenet, sizeof(struct UzenetTartalom), 0);
					if(status < 0) {
						printf("Hiba az uzenet kuldese kozben!\n");
						exit(-3);
					}
					
					hanyadik++;
				} else {
					elozo_hibas = 1;
					char megjegyzes[1024];
					strcpy(megjegyzes, "Van meg");
					
					uzenet.mtype = 1;
					strcpy(uzenet.tartalom.megjegyzes, megjegyzes);
					
					status = msgsnd(uzenetsor, &uzenet, sizeof(struct UzenetTartalom), 0);
					if(status < 0) {
						printf("Hiba az uzenet kuldese kozben!\n");
						exit(-3);
					}
				}
			}
		}
		
        waitpid(child, &status, 0);
    
		printf("[SZULO] Osszesen %d szerelvenyt kellett inditani!\n", osszes);
	} else {
        // Gyerek - szerelveny
		int parent = getppid();
		
		int run = 1;
		while(run == 1) {
			struct Uzenet uzenet;
			status = msgrcv(uzenetsor, &uzenet, sizeof(struct UzenetTartalom), 1, 0);
			if(status < 0) {
				printf("Hiba az uzenet fogadasa kozben!\n");
				exit(-3);
			}
			
			char tipus[100];
			if(uzenet.tartalom.pot == 1) {
				strcpy(tipus, "POT-SZERELVENY");
			} else {
				strcpy(tipus, "SZERELVENY");
			}
			
			printf("[%s] Megkapta az uzenetet!\n\n", tipus);
			
			// Kisorsolni, hogy sikeresen teljesitette-e az uzemet
			int sikeres = 1; // 0 - sikertelen, 1 - sikeres
			int random = (rand() * getpid()) % 10;
			
			if(random == 0) {
				sikeres = 0;
			}
			
			// Kiíratni a megfelelo dolgokat
			int hanyadik = uzenet.tartalom.hanyadik;
			int max = uzenet.tartalom.max;
			int hatralevo = max - hanyadik;
			
			printf("[%s] Hanyadik: %d\n", tipus, hanyadik);
			printf("[%s] Hatralevo: %d\n", tipus, hatralevo);
			printf("[%s] Teljesitette-e: %d\n", tipus, sikeres);
			
			// Eljut a vegallomasra
			sleep(1);
			printf("[%s] Jelzest kuld!\n\n", tipus);
			kill(parent, SIGUSR1);
			
			char ok[1024];
			if(sikeres == 1) {
				strcpy(ok, "Sikeresen beert!");
			} else {
				random = rand() % 2;
				if(random == 0) {
					strcpy(ok, "Nem ert be. Oka: elcsuszott egy bananhejon!");
				} else {
					strcpy(ok, "Nem ert be. Oka: lefagyott a zord telben!");
				}
			}
			
			struct UzenetTartalom tartalom;
			strcpy(tartalom.megjegyzes, ok);
			
			uzenet.mtype = 2;
			uzenet.tartalom = tartalom;
			
			// Elkuldi a megfelelo uzenetet
			printf("[%s] Elkuldi az uzenetet!\n", tipus);
			status = msgsnd(uzenetsor, &uzenet, sizeof(struct UzenetTartalom), 0);
			if(status < 0) {
				printf("Hiba az uzenet kuldese kozben!\n");
				exit(-3);
			}
			
			// Megkapja a szulotol, hogy van-e meg tobb
			status = msgrcv(uzenetsor, &uzenet, sizeof(struct UzenetTartalom), 1, 0);
			if(status < 0) {
				printf("Hiba az uzenet fogadasa kozben!\n");
				exit(-3);
			}
			
			if(strcmp(uzenet.tartalom.megjegyzes, "Nincs mar") == 0) {
				// Nincs mar
				run = 0;
			}
		}
        exit(0);
    }
    
    printf("Vege\n");
	return 0;
}