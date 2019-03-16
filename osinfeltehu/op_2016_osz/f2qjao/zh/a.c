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
        int max = atoi(argv[1]);
		
		int hanyadik = (rand() % (max-1)) + 1;
		
		printf("[SZULO] max: %d\n", max);
		printf("[SZULO] random: %d\n", hanyadik);
		
		struct UzenetTartalom tartalom = {hanyadik, max, ""};
		struct Uzenet uzenet = {1, tartalom};
		
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
		printf("[SZULO] Uzenet: %s\n", uzenet.tartalom.megjegyzes);
		
        waitpid(child, &status, 0);
    } else {
        // Gyerek - szerelveny
		int parent = getppid();
		
		struct Uzenet uzenet;
        status = msgrcv(uzenetsor, &uzenet, sizeof(struct UzenetTartalom), 1, 0);
        if(status < 0) {
            printf("Hiba az uzenet fogadasa kozben!\n");
            exit(-3);
        }
		
		printf("[SZERELVENY] Megkapta az uzenetet!\n\n");
		
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
		
		printf("[SZERELVENY] Hanyadik: %d\n", hanyadik);
		printf("[SZERELVENY] Hatralevo: %d\n", hatralevo);
		printf("[SZERELVENY] Teljesitette-e: %d\n", sikeres);
		
		// Eljut a vegallomasra
		sleep(1);
		printf("[SZERELVENY] Jelzest kuld!\n\n");
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
		
		struct UzenetTartalom tartalom = {-1, -1, ""};
		strcpy(tartalom.megjegyzes, ok);
		
		uzenet.mtype = 2;
		uzenet.tartalom = tartalom;
		
		// Elkuldi a megfelelo uzenetet
		printf("[SZERELVENY] Elkuldi az uzenetet!\n");
		status = msgsnd(uzenetsor, &uzenet, sizeof(struct UzenetTartalom), 0);
        if(status < 0) {
            printf("Hiba az uzenet kuldese kozben!\n");
            exit(-3);
        }
		
        exit(0);
    }
    
    printf("Vege\n");
	return 0;
}