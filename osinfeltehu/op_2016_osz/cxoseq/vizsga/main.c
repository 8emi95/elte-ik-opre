#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAX_SIZE 512
#define READ 0
#define WRITE 1


void handler(int signumber)
{
	//printf("Signal sent!!");
	return;
}

void trenerThread();
void examineeThread();

pid_t childPid;
char *username;
int pipeParent[2];
int pipeChild[2];

int main(int argc, char ** argv) {

    signal(SIGUSR1,handler);
	
	srand ( time(NULL) );
	
    if (argc != 2) {
        printf("Parameter count not match!");
        exit(EXIT_FAILURE);
    }

    username = argv[1];

    if (pipe(pipeParent) == -1 || pipe(pipeChild) == -1)
    {
        perror("Error Opening Pipe!\n");
        exit(EXIT_FAILURE);
    }

	childPid = fork();

	if (childPid == -1) { // main
		perror("Fork hiba\n");
		exit(EXIT_FAILURE);
	}

	if (childPid > 0) { // main
		trenerThread();
	} else { // child
		examineeThread();
	}
	
    return 0;
}

void trenerThread() {
	printf("Hello, I'm trener!!\n");
	
    close(pipeParent[READ]);
    close(pipeChild[WRITE]);
	int success;
	int number = 1;
	
	write(pipeParent[WRITE], username, strlen(username));
	pause();
	write(pipeParent[WRITE], "FELADAT", 8);
	pause();
	write(pipeParent[WRITE], &number, sizeof(number));
	pause();
	
	if (read(pipeChild[READ], &success, sizeof(int)) > 0) {
		if (success >= 100) {
			printf("The examinee [%d] solved\n", number);
		} else {
			printf("The examinee [%d] didn't solved\n", number);
		}
		
	}
	
    close(pipeParent[WRITE]);
    close(pipeChild[READ]);
	
	wait();
}


void examineeThread() {
	printf("Hello, I'm examinee!!\n");
    close(pipeChild[READ]);
    close(pipeParent[WRITE]);
	
	char receivedMessage[MAX_SIZE];
	char receivedExercise[MAX_SIZE];
	int number;
	
	if (read(pipeParent[READ], receivedMessage, sizeof(receivedMessage)) > 0) {
		printf("My Name is %s\n", receivedMessage);
		kill(getppid(), SIGUSR1);
	}
	kill(getppid(), SIGUSR1);
	if (read(pipeParent[READ], receivedExercise, sizeof(receivedExercise)) > 0) {
		printf("My Excercise is %s\n", receivedExercise);
	}
	kill(getppid(), SIGUSR1);
	
	if (read(pipeParent[READ], &number, sizeof(int)) > 0) {
		printf("My number is %d\n", number);
	}
	int success = (rand() % 1000);
	
	if (success >= 100) {
		printf("Exercise solved\n");
	} else {
		printf("Exercise unsolved\n");
	}
	write(pipeChild[WRITE], &success, sizeof(success));
	
	kill(getppid(), SIGUSR1);
	
    close(pipeChild[WRITE]);
    close(pipeParent[READ]);
}
