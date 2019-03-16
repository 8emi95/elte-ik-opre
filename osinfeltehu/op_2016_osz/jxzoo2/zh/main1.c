#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>

int szerelvenyszam[] = {000, 111, 222, 333, 444};
int osszes_szerelveny_szama = 5;
pid_t child; 
int uzenetsor, status, i; 
key_t kulcs;

struct uzenet{ 
    long mtype;
	int szerelvenyszam;
}; 

int fogad(int uzenetsor);
int kuld(int uzenetsor, int szerelvenyszam);
//void handler(int sign);

int main(int argc, char** argv){
	if (argc < 2) {
		perror("Legalabb egy argumentumot adj meg!");
		exit(-1);
	}
	
	int n = atoi(argv[1]); //max szerelvenyszam
	
	kulcs = ftok(argv[0],1); 
	uzenetsor = msgget(kulcs, 0600 | IPC_CREAT); //open v create
	if (uzenetsor < 0){ 
		perror("msgget"); 
		return 1; 
	} 
	
	//signal(SIGUSR1, handler);
	
	child = fork();
	
	if (child > 0){ //szulo
		wait( NULL ); 
		/*int rnd = random()%osszes_szerelveny_szama;
		int baleset = random()%10;
		//printf("random szerelveny%d\n", rnd);
		for (i = 0; i < n; i++){
		kuld(uzenetsor, szerelvenyszam[rnd]);
		}
		return 0;*/
		for (i = 0; i < n; i++) {
			int szerelvenyszam = fogad(uzenetsor);
			printf("%d. szerelvenyszam: %d\n", (i+1), szerelvenyszam);
		}
		status = msgctl(uzenetsor, IPC_RMID, NULL); 
		if (status < 0) 
			perror("msgctl"); 
		return 0;
	} else if (child == 0){ //gyerek 
		/*for (i = 0; i < n; i++) {
			int szerelvenyszam = fogad(uzenetsor);
			printf("%d . gyerek, szerelvenyszam: %d osszes szerelveny szama: %d\n", i+1, szerelvenyszam, osszes_szerelveny_szama);
		}
		// After terminating child process, the message queue is deleted. 		
		status = msgctl(uzenetsor, IPC_RMID, NULL);
		if (status < 0)
			perror("msgctl");
		return 0;*/
		for (i = 0; i < n; i++) {
			int rnd = random()%osszes_szerelveny_szama;
			kuld(uzenetsor, szerelvenyszam[rnd]);
		}
		//kill(getppid(),SIGUSR1);
	} else {
		perror("fork"); 
		return 1; 
	}
	return 0;
}

int kuld(int uzenetsor, int szerelvenyszam){
	const struct uzenet uz = {5, szerelvenyszam};
	int status = msgsnd(uzenetsor, &uz, sizeof(uz), 0);
	if (status < 0){
		perror("msgsnd"); 
	}
	return 0; 
}

int fogad(int uzenetsor){ 
     struct uzenet uz; 
     int status = msgrcv(uzenetsor, &uz, sizeof(uz), 5, 0); 
     if (status < 0) {
          perror("msgsnd"); 
     } else {
     		//
     		return uz.szerelvenyszam;
     }
     return -1; 
} 