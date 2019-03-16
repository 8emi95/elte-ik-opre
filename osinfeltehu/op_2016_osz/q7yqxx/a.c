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
	printf("Jelzes kuldese megtortent\n",signumber);
}

// uzenetkuldes
int kuld( int uzenetsor ) {
	sleep(1);
	
	struct uzenet uz = { 5, "Ki a legjobb csapat?!" };
	int status;
	
	// uzenetsor
	status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	if ( status < 0 ) 
	{ 
		perror("msgsnd");
	}
	return 0; 
}

// uzenet fogadas
int fogad( int uzenetsor ) {
	struct uzenet uz; 
	int status;
	status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
	if (status < 0) 
	{
		perror("msgsnd");
	}
	else
	{
		printf( "A kapott kerdes:  %s\n", uz.mtext ); 
	}
	return 0; 
}

int main(int argc, char* argv[]) 
{
	srand(time(NULL));
	int uzenetsor, status;
	key_t kulcs;
	
	kulcs = ftok(argv[0],1); 
	printf ("A kulcs: %d\n",kulcs);
	uzenetsor = msgget(kulcs, 0600 | IPC_CREAT); 
	if (uzenetsor < 0) { 
		perror("msgget"); 
		return 1; 
	}
	
	signal(SIGTERM, handler);

	pid_t child = fork();
	if (child > 0) {
		pid_t child2 = fork();
		if(child2 == 0) {
			printf("A 2. hallgato beert az orara\n");
			kill(getppid(), SIGTERM);
			sleep(1);
			fogad(uzenetsor);
			sleep(1);
			printf("A 2. hallgato befejezte a napot\n");
		}
		else {
			pause();
			pause();
			printf("A hallgatok keszek megkapni a kerdest\n");
			// Uzenetsor kuldese
			kuld(uzenetsor);
			kuld(uzenetsor);
			wait(NULL);
			status = msgctl( uzenetsor, IPC_RMID, NULL ); 
			if ( status < 0 ) {
				perror("msgctl"); 
			}
			
			int status;
			wait(&status);
			printf("Az eloadasnak vege, menjetek bekevel!\n");
		}
	}
	else {
		printf("A 1. hallgato beert az orara\n");
		sleep(2);
		kill(getppid(), SIGTERM);
		fogad(uzenetsor);
		sleep(2);
		printf("A 1. hallgato befejezte a napot\n");
	}
	return 0;
}