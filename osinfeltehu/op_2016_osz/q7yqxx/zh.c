#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <wait.h>

struct uzenet { 
	long mtype;
	char mtext [ 1024 ]; 
};

void handler(int signumber){
	printf("Vegallomas.\n",signumber);
}

// uzenet fogadas
int fogad( int uzenetsor ) {
	struct uzenet uz; 
	int status;
	status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
	if (status < 0) {
		perror("msgsnd");
	}
	else{
		printf( "Ma a %s. vagyok.\n", uz.mtext ); 
	}
	return 0; 
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	int uzenetsor, status;
	int szerelvenyszam = atoi(argv[0]);
	key_t kulcs;
	kulcs = ftok(argv[0],1); 
	uzenetsor = msgget(kulcs, 0600 | IPC_CREAT); 
	if (uzenetsor < 0) {
		perror("msgget"); 
		return 1; 
	}
	
	signal(SIGTERM, handler);

	pid_t child = fork();
	
	//szülõ
	if (child > 0) {
		// Uzenetsor kuldese
		srand(time(NULL));
		int rndm = rand() % 100;
		char randomNumber[5];
		sprintf(randomNumber, "%d", rndm);

		struct uzenet uz = { 5, "szerelvenyszam" };
		int status, uzenetsor;
		
		// uzenetsor
		status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
		if ( status < 0 ) { 
			perror("msgsnd");
		}
		status = msgctl( uzenetsor, IPC_RMID, NULL ); 
		if ( status < 0 ) {
			perror("msgctl"); 
		}
		
		wait(&status);
		kill(getpid(), SIGTERM);
	}
	//gyerek
	else {
		struct uzenet uz; 
		int status,uzenetsor;
		status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
		 
		if (status < 0) {
			perror("msgsnd");
		}
		else{
			printf( "Ma a %s. vagyok.\n", uz.mtext ); 
		}
	}
	return 0;
}