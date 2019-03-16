#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <unistd.h>

#define MAX_SIZE 512
#define READ 0
#define WRITE 1

FILE *eventFile = NULL;

int eventFileCount = 1;
int val, code;
int pipeParent[2]; // unnamed pipe file descriptor array
int pipeChild[2];
pid_t childPid;
char *message;
struct sigevent notify;

void partnerThread();
void companyThread(char *filename);

void handler(int signumber)
{
    //printf("Signal SENT %d ++++++++++++", signumber);
	return;
}

int openEventFile(char filename[]) {
	eventFile = fopen(filename, "r");
	if (NULL != eventFile) {
		rewind(eventFile);
		return 1;
	} else {
		return 0;
	}
}

void closeEventFile() {
	if (NULL != eventFile) {
		fclose(eventFile);
	}
}

char *readEventFile(char *line) {
	return fgets(line, MAX_SIZE, eventFile);
}

int main(int argc, char ** argv) {
    if (argc == 2) {
        int argFileCount = atoi(argv[1]);
        if (argFileCount > 0) {
            eventFileCount = argFileCount;
        }
    }

    signal(SIGUSR1,handler);

    int i;
    for(i = 1; i <= eventFileCount; ++i ) {
        char filename[50];
        sprintf(filename, "guests%d.csv", i);

        openEventFile(filename);
        if (eventFile) {

            if (pipe(pipeParent) == -1 || pipe(pipeChild) == -1)
            {
                perror("Hiba a pipe nyitaskor!\n");
                exit(EXIT_FAILURE);
            }

            childPid = fork();

            if (childPid == -1) { // main
                perror("Fork hiba\n");
                exit(EXIT_FAILURE);
            }

            if (childPid > 0) { // main
                companyThread(filename);
            } else { // child
                partnerThread();
            }
        } else {
            printf("File not accessible: [%s]\n", filename);
        }
    }

	return 0;
}

void companyThread(char *filename) {
   int status;
    printf("Company starting event [%s]\n", filename);
    close(pipeParent[READ]);
    close(pipeChild[WRITE]);


    openEventFile(filename);
    if (!eventFile) {
        printf("File is missing\n");
        exit(EXIT_FAILURE);
    }
    char *readResult;

    // read first row: place of event
    char line[MAX_SIZE] = "";
    readResult = readEventFile(line);
    if (readResult) {
        //send event's place
        write(pipeParent[WRITE], line, strlen(line));
        printf("Event's place [%s] sent. Wait for Guide to travel there.\n\n", strtok(line, "\n"));
        pause();
        while (readResult) {
            readResult = readEventFile(line);
            if (readResult) {
                write(pipeParent[WRITE], line, strlen(line));
                pause();
            }
        }
    }

    closeEventFile();
    close(pipeParent[WRITE]);

    //char successPercent[MAX_SIZE];
    int successPercent;
    read(pipeChild[READ], &successPercent, sizeof(int));
    printf("The event succcessfulness: %d\n", successPercent);
    close(pipeChild[READ]);
    wait(&status);
    printf("Company stoped the event [%s]\n", filename);
}

void partnerThread() {
    close(pipeParent[WRITE]);
    close(pipeChild[READ]);

    srand ( time(NULL) );
    int guestsCount = 0;
    char guests[MAX_SIZE][MAX_SIZE];
    printf("Partner started !! \n");
    char eventPlace[MAX_SIZE];
    char receivedMessage[MAX_SIZE];

    if (read(pipeParent[READ], receivedMessage, sizeof(receivedMessage)) > 0) {
        strcpy(eventPlace, strtok(receivedMessage, "\n"));
        printf("Travelling to: %s\n", eventPlace);
        // Travelling time
        sleep(1);
        kill(getppid(), SIGUSR1);
        while (read(pipeParent[READ], receivedMessage, sizeof(receivedMessage)) > 0) {
            strcpy(receivedMessage, strtok(receivedMessage, "\n"));
            int chance = (rand() % 1000);
            if (chance > 100) {
                printf("Guest IS Arrived: %s\n", receivedMessage);
                strcpy(guests[guestsCount], receivedMessage);
                ++guestsCount;
            } else {
                printf("Guest DOES NOT Arrived: %s\n", receivedMessage);
            }
            kill(getppid(), SIGUSR1);
        }

        printf("Arrived guests: %d\n", guestsCount);
    }

    int success = (rand() % 100);
    write(pipeChild[WRITE], &success, sizeof(int));

    close(pipeParent[READ]);
    close(pipeChild[WRITE]);
}
