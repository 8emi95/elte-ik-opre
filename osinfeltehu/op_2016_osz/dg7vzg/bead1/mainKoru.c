#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>




struct uzenet { 
	long mtype; 
	char mtext[1024];
}; 


// Üzenetet küld az üzenetsorba. 
void kuld(int uzenetsor, char *data) { 
	struct uzenet uz; 
	int status; 


	uz.mtype = 5;
	strcpy(uz.mtext, data);
	msgsnd(uzenetsor, &uz, strlen(data), IPC_NOWAIT);


	//msgsnd( uzenetsor, &uz, strlen(uz.mtext), 0 );
	/*printf("KULD A kuldott uzenet(mtext): %s\n",uz.mtext);
	printf("KULD A kuldott uzenet(data): %s\n",data);*/


}


// Üzenetet fogad az üzenetsorból. 
int fogad(int uzenetsor, char* data) { 
	struct uzenet uz; 
	int status;
	
	msgrcv(uzenetsor, &uz, 1024, 5, 0);


	strcpy(data, uz.mtext);
	//printf("FOGAD A kuldott uzenet: %s\n",uz.mtext);


	return 0;
} 


int main(int argc, char *argv[]) {
	srand (time(NULL));//random szám generálásához
	int pipefd1[2]; // unnamed pipe file descriptor array
	int pipefd2[2]; // unnamed pipe file descriptor array
	
	if (pipe(pipefd1) == -1) {
		perror("Hiba a pipe1 nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	if (pipe(pipefd2) == -1) {
		perror("Hiba a pipe2 nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	key_t kulcs;
	kulcs = ftok(argv[0],1);
	int uzenetsor1 = msgget( kulcs, 0700 | IPC_CREAT );
	int uzenetsor2 = msgget( kulcs, 0700 | IPC_CREAT ); 
	
	if ( uzenetsor1 < 0 ) {
		perror("msgget hiba"); 
		exit(EXIT_FAILURE);
	}
	if ( uzenetsor2 < 0 ) {
		perror("msgget hiba"); 
		exit(EXIT_FAILURE);
	} 
	
	pid_t child1;
	child1 = fork();	// creating parent-child processes
	if (child1 == -1) {
		perror("Fork hiba");
		exit(EXIT_FAILURE);
	}


	if (child1 == 0) { // 1.gyerek folyamat
		close(pipefd1[0]);
		char* kapottUzenet;
		int status;
		int ertekeles;
		status = fogad(uzenetsor1, &kapottUzenet);
		printf("A kapott kerdes: %s\n",&kapottUzenet);
		sleep(1);
		ertekeles = (rand() % 4) + 1;
		write(pipefd1[1],&ertekeles, sizeof(ertekeles));
		printf("Gyerek 1 befejezte a működését!\n");
		close(pipefd1[1]);


	} else {    // szulo folyamat
		pid_t child2;
		child2 = fork();
		
		if (child2 == -1) {
			perror("Fork hiba");
			exit(EXIT_FAILURE);
		}
		
		if (child2 == 0) { //2. gyerek folyamat
		close(pipefd2[0]);
		char* kapottUzenet;
		int status;
		int ertekeles;
		status = fogad(uzenetsor2, &kapottUzenet);
		printf("A kapott kerdes: %s\n",&kapottUzenet);
		sleep(1);
		ertekeles = (rand() % 4) + 1;
		write(pipefd2[1],&ertekeles, sizeof(ertekeles));
		printf("Gyerek 2 befejezte a működését!\n");
		close(pipefd2[1]);


		} else {//szülő folyamat*/
			close(pipefd1[1]);
			close(pipefd2[1]);
			char kuldottUzenet[] = "Van kérdés? ";
			int ertekeles1, ertekeles2;
			
			kuld(uzenetsor1, &kuldottUzenet);
			kuld(uzenetsor2, &kuldottUzenet);
			read(pipefd1[0],&ertekeles1,sizeof(ertekeles1));
			printf("Elso gyerek valasza: %d\n",ertekeles1);
			read(pipefd2[0],&ertekeles2,sizeof(ertekeles2));
			printf("Masodik gyerek valasza: %d\n",ertekeles2);
			
			
			wait();
			close(pipefd1[0]);
			close(pipefd2[0]);
		}
	}
}
