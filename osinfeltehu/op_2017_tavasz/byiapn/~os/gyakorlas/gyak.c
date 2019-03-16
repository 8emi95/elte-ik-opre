#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>
#include <unistd.h>

struct uzenet {
	long _kat;
	char _uz[1024];
};

void main(int argc, char* argv[]) {
	key_t kulcs = ftok(argv[0], 1);
	int msgid = msgget(kulcs, 0600 | IPC_CREAT);
	pid_t child = fork();
	if(child != 0) {
		const struct uzenet uz = { 1, "Kutya" };
		msgsnd(msgid, &uz, strlen(uz._uz)+1, 0);
		printf("szulo: %s\n", uz._uz);
		waitpid(child, NULL, 0);
		msgctl(msgid, IPC_RMID, NULL);
	} else {
		struct uzenet uz2;
		msgrcv(msgid, &uz2, 1024, 0, 0);
		printf("gyerek: %s\n", uz2._uz);
	}
}