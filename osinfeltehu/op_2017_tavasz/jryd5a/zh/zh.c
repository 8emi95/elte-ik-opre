#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct uzenet { 
    long mtype; 
    int pid;
};

size_t msglength = sizeof(struct uzenet) - sizeof(long);

int kuld_pid(int uzenetsor, int pid) {
	const struct uzenet uz = {2, pid};
	int status = msgsnd(uzenetsor, &uz, msglength, 0);
	if (status < 0)
		perror("msgsend error");
	
	return 0;
}

int fogad_pid(int uzenetsor) {
	struct uzenet uz;
	int status;
	
	status = msgrcv(uzenetsor, &uz, msglength, 0, 0);
	if (status < 0) {
		perror("msgsend error");
	}

	return uz.pid;
}

void telefonFogadas(int sig) {
	printf("vasarlo: fogadom a hivast\n");
}

int main(int argc, char ** args) {
	int ugynok_pid;
	int vasarlo_pid;
	int uzenetsor, status;
	key_t kulcs;
	
	kulcs = ftok(args[0], 1);
	uzenetsor = msgget(kulcs, 0700 | IPC_CREAT);
	
	ugynok_pid = fork();
	if (0 == ugynok_pid) {
		vasarlo_pid = fogad_pid(uzenetsor);
		printf("ugynok: fogadtam uzenetsoron a vasarlo telefonszamat: %d\n", vasarlo_pid);
		printf("ugynok: felhivom a vasarlot...\n");
		kill(vasarlo_pid, SIGUSR1);
		wait(vasarlo_pid);
	} else {
		signal(SIGUSR1, telefonFogadas);
		vasarlo_pid = fork();
		if (0 == vasarlo_pid) {
			pause();
		} else {
			kuld_pid(uzenetsor, vasarlo_pid);
			wait(NULL);
		}
	}

	return 0;
}

