#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 


struct uzenet { 
    long mtype;	// Oszt�lyoz�sra haszn�lhat�
    char mtext [ 1024 ]; 
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
	 
	if(fork < 0) {
		printf("Nem sikerult a forkolas!\n");
		exit(-1);
	}
	
	if(fork > 0) {
		// Sz�l� - k�ldi az �zenetet
		struct uzenet uz = {5, "Szia!"};
		status = msgsnd(uzenetsor, &uz, strlen(uz.mtext) + 1, 0);
		if(status < 0) {
			printf("Hiba az uzenet kuldese kozben!\n");
			exit(-3);
		}
		
		waitpid(child, &status, 0);
		
		msgctl(uzenetsor, IPC_RMID, NULL);
	} else {
		// Gyerek - fogadja az �zenetet
		struct uzenet uz;
		status = msgrcv(uzenetsor, &uz, 1024, 5, 0); // 5, hogy az 5-os �zenetekb�l veszi a k�vetkez�t
		if(status < 0) {
			printf("Hiba az uzenet fogadasa kozben!\n");
			exit(-3);
		}
		
		printf("Uzenet: \'%s\'\n", uz.mtext);
	}
    
	return 0; 
} 
