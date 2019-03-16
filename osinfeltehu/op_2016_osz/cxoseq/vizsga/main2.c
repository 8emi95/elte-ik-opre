#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAX_SIZE 512
#define SUCCESS_BORDER 500
#define SUCCESS_BORDER2 800
#define READ 0
#define WRITE 1


void handler(int signumber)
{
	//printf("Signal sent!!");
	return;
}

int trenerThread(char *uname, int number, int try);
void examineeThread(int try);
void startExam(char **examinees);
pid_t childPid;
char *username;
int pipeParent[2];
int pipeChild[2];
char *failedExaminees[MAX_SIZE];
int examineeCount;
int failedExamineeCount;

int main(int argc, char ** argv) {

    signal(SIGUSR1,handler);
	examineeCount = argc;
	failedExamineeCount = 0;
	
	srand ( time(NULL) );
	
    if (argc < 2) {
        printf("Parameter count not match!");
        exit(EXIT_FAILURE);
    }

	startExam(argv);
	printf("Failed examinees %d", failedExamineeCount);
	
    return 0;
}

void startExam(char **examinees) {
	int i;
	for (i = 1; i < examineeCount; ++i) {
		int try;
		for (try = 0; try < 2; ++try) 
		{
			if (pipe(pipeParent) == -1 || pipe(pipeChild) == -1)
			{
				perror("Error Opening Pipe!\n");
				exit(EXIT_FAILURE);
			}
			
			username = examinees[i];
			childPid = fork();

			if (childPid == -1) {
				perror("Fork hiba\n");
				exit(EXIT_FAILURE);
			}

			if (childPid > 0) { // main
				int status;
				int retry = trenerThread(username, i, try);
				printf("\n\n\n");
				if (retry == 0) {
					try = 2;
				} else {
					if (try > 0) {
						failedExaminees[failedExamineeCount] = username;
						++failedExamineeCount;
					}
				}
				
			} else { // child
				examineeThread(try);
				i = examineeCount;
			}
		}
	}
}

int trenerThread(char * uname, int number, int try) {
	int succesBorder = (try == 0) ? SUCCESS_BORDER : SUCCESS_BORDER2;
	printf("Hello, I'm trener!!\n");
	
    close(pipeParent[READ]);
    close(pipeChild[WRITE]);
	int success;
	
	write(pipeParent[WRITE], uname, strlen(uname));
	pause();
	write(pipeParent[WRITE], "FELADAT", 8);
	pause();
	write(pipeParent[WRITE], &number, sizeof(number));
	pause();
	
	if (read(pipeChild[READ], &success, sizeof(int)) > 0) {
		if (success >= succesBorder) {
			printf("The examinee [%d] solved\n", number);
		} else {
			printf("The examinee [%d] didn't solved\n", number);
		}
	}
	
    close(pipeParent[WRITE]);
    close(pipeChild[READ]);
	wait(NULL);
	
	return (success <= succesBorder) ? 1 : 0;
}


void examineeThread(int try) {
	int succesBorder = (try == 0) ? SUCCESS_BORDER : SUCCESS_BORDER2;
	printf("Hello, I'm examinee!!\n");
    close(pipeChild[READ]);
    close(pipeParent[WRITE]);
	
	char receivedMessage[MAX_SIZE];
	char receivedExercise[MAX_SIZE];
	int examineeNumber;
	
	if (read(pipeParent[READ], receivedMessage, sizeof(receivedMessage)) > 0) {
		printf("My Name is %s\n", receivedMessage);
	}
	kill(getppid(), SIGUSR1);
	if (read(pipeParent[READ], receivedExercise, sizeof(receivedExercise)) > 0) {
		printf("My Excercise is %s\n", receivedExercise);
	}
	kill(getppid(), SIGUSR1);
	
	if (read(pipeParent[READ], &examineeNumber, sizeof(int)) > 0) {
		printf("My number is %d\n", examineeNumber);
	}
	kill(getppid(), SIGUSR1);
	
	int success = (rand() % 1000);
	if (success >= succesBorder) {
		printf("I solved the exercise.\n");
	} else {
		printf("I do not solved the exercise.\n");
	}
	write(pipeChild[WRITE], &success, sizeof(success));

    close(pipeChild[WRITE]);
    close(pipeParent[READ]);
}
