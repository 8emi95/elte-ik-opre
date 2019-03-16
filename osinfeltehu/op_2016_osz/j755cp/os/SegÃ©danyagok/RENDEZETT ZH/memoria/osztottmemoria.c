#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main(int argc, char** argv) {
	key_t key = ftok(argv[0],1);
	int shmid = shmget(key,25,IPC_CREAT|0600);
	int status;
	pid_t child = fork();
	if(child>0) {
		char* add = shmat(shmid,NULL,0);
		strcpy(add,"Hello\n");
		shmdt(add);
		wait(&status);
		shmctl(shmid,IPC_RMID,0); //törlés
	} else {
		char* add = shmat(shmid,NULL,0);
		printf("%s\n",add);
		shmdt(add);
	}


	return 0;
}