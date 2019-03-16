#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/msg.h>
#include <signal.h>
#include <time.h>
#include <math.h>

typedef struct msgbuf {
	long mtype;
    int osszesszerelvenyszam;
    int szerelvenyszama;
	char halaloka[80];
} msgbuf;

void handler(int signumber)
{
	if (signumber == SIGUSR1)
		printf("Bevontattak :(\n");
	else {
		printf("Beertem! :)\n");			
	}
}

int main(int argc, char* argv[])
{
	int status;		
	int szerelvenyekszama;
	int szerelvenyszam;
	
	signal(SIGUSR1,handler);
	signal(SIGUSR2,handler);

	szerelvenyekszama = atoi(argv[1]);
	printf("%d\n", szerelvenyekszama);
	msgbuf buffer;
	
	srand(time(NULL));
	szerelvenyszam = rand() % szerelvenyekszama + 1;
	
	buffer.mtype = 2;
	buffer.osszesszerelvenyszam = szerelvenyekszama;
	buffer.szerelvenyszama = szerelvenyszam;
	
	int queueid;

	queueid = msgget((key_t)1234, 0666 | IPC_CREAT);
	
	if (queueid == -1) {
	  perror("msgget failed with error");
	  exit(EXIT_FAILURE);
	}

	msgsnd(queueid, &buffer, sizeof(msgbuf), 0);
	
	pid_t pid;
	pid_t child = fork();
	if(child < 0) {
		perror("Fork error");
		exit(1);
	}
	else if (child > 0) { //parent process
		msgbuf uzenete;
		pause();
		sleep(1);
		if (msgrcv(queueid, (void *)&uzenete, sizeof(msgbuf), 2, 0) == -1) {
			perror("msgcrv failed with error");
			exit(EXIT_FAILURE);
		}
		
		printf("A szerelveny uzenete: %s", uzenete.halaloka);
		waitpid(child, &status, 0);

	}
	else { //child process	
		msgbuf childbuffer;
		int halal;
		int halott = 0;		

		if (msgrcv(queueid, (void *)&childbuffer, sizeof(msgbuf), 2, 0) == -1) {
			perror("msgcrv failed with error");
			exit(EXIT_FAILURE);
		}
		printf("En ma a %d. szerelveny vagyok a %d-bol. Meg %d szerelveny jon utanam. \n", childbuffer.szerelvenyszama, childbuffer.osszesszerelvenyszam, childbuffer.osszesszerelvenyszam-childbuffer.szerelvenyszama);
		halal = rand() % 10 + 1;
		if (halal == 10)
		{
			halott = 1;
			strcpy(childbuffer.halaloka, "Megfagytam, vagy elcsusztam egy bananhejon. :/ \n");
			kill(getppid(), SIGUSR1);
		} else 
		{
			halott = 0;
			strcpy(childbuffer.halaloka, "Semmi gond nem tortent! :) \n");
			kill(getppid(), SIGUSR2);
	}
	msgsnd(queueid, &childbuffer, sizeof(childbuffer), 0);

	}
	return 0;
}