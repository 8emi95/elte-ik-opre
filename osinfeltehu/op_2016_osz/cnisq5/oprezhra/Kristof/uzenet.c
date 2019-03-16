#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

struct UzenetTartalom {
	int a;
	char op;
	int b;
	char szoveg[1024];
};

struct Uzenet { 
    long mtype;	// Oszt�lyoz�sra haszn�lhat�
    struct UzenetTartalom tartalom; 
}; 

int main (int argc, char* argv[]) { 
    int status;
	
	key_t kulcs = ftok(argv[0],1); // Kulcs gener�l�sa, amely az osztott mem�ri�hoz fog adni egy egyedi kulcsot, amit az OS fog kezelni
	int uzenetsor = msgget(kulcs, IPC_CREAT|S_IRUSR|S_IWUSR);
	
	if(uzenetsor < 0) {
		printf("Hiba az uzenetsor letrehozasakor!\n");
		exit(-2);
	}
	
	int child = fork();
	
	if(child < 0) {
		printf("Nem sikerult a forkolas!\n");
		exit(-1);
	}
	
	if(child > 0) {
		// Sz�l� - k�ldi az �zenetet
		printf("Szulo\n");
		sleep(5);
		struct UzenetTartalom tartalom = {5, '+', 5, "Egyenlo 10-zel!"};
		struct Uzenet uzenet = {5, tartalom};
		status = msgsnd(uzenetsor, &uzenet, sizeof(struct UzenetTartalom), 0);
		if(status < 0) {
			printf("Hiba az uzenet kuldese kozben!\n");
			exit(-3);
		}
		
		waitpid(child, &status, 0);
		
		msgctl(uzenetsor, IPC_RMID, NULL);
	} else {
		// Gyerek - fogadja az �zenetet
		printf("Gyerek\n");
		struct Uzenet uzenet;
		status = msgrcv(uzenetsor, &uzenet, sizeof(struct UzenetTartalom), 5, 0); // 5, hogy az 5-os �zenetekb�l veszi a k�vetkez�t
		if(status < 0) {
			printf("Hiba az uzenet fogadasa kozben!\n");
			exit(-3);
		}
		
		printf("Uzenet: \'%s\'\n", uzenet.tartalom.szoveg);
		exit(0);
	}
    
	return 0; 
} 
