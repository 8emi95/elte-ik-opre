#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define BS 1024



struct Mess {
	long id;
	char text[BS];
	char nev[BS];
};

void error(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void CotG(char *arg) {
	struct Mess ms;
	key_t queueKey(arg,1);
	int queueID = msgget(queueKey, 0600);
	int status = msgrcv(queueID, &ms, BS, 0, 0);
    if(status < 0) {
        error("Error recv from queue");
    }
	
	printf("CHILD - EZT KAPTAM: %s\n", messageStruct.text);
    sleep(2);
	
	int rng = rand() % 2;
	struct Mess k;
	if (rng == 0) {
		printf("CHILD - SIKER");
		k = {1, "SIKER", ms.nev};
	} else {
		printf("CHILD - SIKERTELEN");
		k = {1, "SIKERTELEN", ms.nev};
	}
	sendCause(queueID2,k);
}

int main(int argc, char *argv[]) {
	pid_t children[argc];
	
	key_t queueKey = ftok(argv[0],1);
	key_t queueKey = ftok(argv[0],1);
    int queueID = msgget(queueKey, 0600 | IPC_CREAT);
	int queueID2 = msgget(queueKey, 0600 | IPC_CREAT); 
	
	int pipe_fd[argc];
	if(pipe(pipe_fd) == -1)
    {
        error("Pipe init error");
    }
	
	struct Mess ms[argc];
	
	for (int i = 0; i < argc; ++i) {
		ms.id[i] = i + 1;
		printf("\nFeladat: ");
		char line[BS];
		scanf("%s\n", line);
		strcpy(ms[i].text, line);
		strcpy(ms[i].nev, argv[i]);
		struct Mess temp = ms[i];
		int qStatus = msgsnd(queueID, &temp, BS, 0);
        if(qStatus < 0) {
            error("Sending queue error.");
        }
	}
	
	for (int i = 0; i < argc; ++i) {
		children[i] = fork();
		struct Mess gyerek;
		if (children[i] < 0) {
			error("Fork error\n");
		} else if(children[i] == 0) {
            CotG(argv[0]);
			receiveCause(queueID2, &gyerek);
			printf("PARENT - EZT KAPTAM: %s\n", gyerek.text);
        }
	}
	
	/*int status;
	pid_t pid;
	int tmp = argc;
	
	while(argc > 0) {
		pid = wait(&status);
	}*/
	
	return 0;
}