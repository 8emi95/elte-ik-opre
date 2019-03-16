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
} msgbuf;

int main(int argc, char* argv[])
{
	int szerelvenyekszama;
	int szerelvenyszam;
	
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
	}
	else { //child process	
		msgbuf childbuffer;
		long int msg_to_recieve = 0;
		

		if (msgrcv(queueid, (void *)&childbuffer, sizeof(msgbuf), msg_to_recieve, 0) == -1) {
			perror("msgcrv failed with error");
			exit(EXIT_FAILURE);
		}
		printf("Én ma a %d. szerelvény vagyok a %d-bol. Meg %d szerelveny jon utanam \n", childbuffer.szerelvenyszama, childbuffer.osszesszerelvenyszam, childbuffer.osszesszerelvenyszam-childbuffer.szerelvenyszama);
	
		quality = rand() % 10 + 1;
		if (write(fd[P2_WRITE], &quality, sizeof(quality)) < 0)
		{
			perror("Child: Failed to write response value");
			exit(EXIT_FAILURE);
					}

	
	}
	return 0;
}