#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <errno.h>
#define MSGSIZE 512

void signalHandler(int signumber){
	printf("Signal with number %i has arrived\n",signumber);
}


static key_t key;
struct message {
	long mtype;
	char mtext[1024];
};

int main(int argc, char** argv){
	printf("Budapest Metro Funs\n");
	
	//paraméter: max
	setlocale(LC_ALL, "");
	if (argc<2){
		printf("Adj meg egy számot paraméterként!\n");
		exit(EXIT_FAILURE);
	}
	int max = atoi(argv[1]);
	
	signal(SIGTERM, signalHandler);
	
	//hanyadik?
	srand(time(NULL));
	int num = rand() % max + 1;
	
	int msgqueue = msgget(key, 0600 | IPC_CREAT);

	if (msgqueue < 0 ) {
		perror("msgget");
		exit(EXIT_FAILURE);
	}
	
	
	
	//
	pid_t pid = fork();
	if (pid < 0){
		printf("Fork calling error\n");
		exit(EXIT_FAILURE);
	}
	
	//PARENT PROCESS
	else if (pid > 0){
		printf("parent here\n");
		pause();
		struct message m;
		printf("ideaig?\n");
		msgrcv(msgqueue, &m, MSGSIZE, 2, 0);
		printf("UZENET: %s", m.mtext);
	//CHILD PROCESS
	} else {
		struct message m = { 2, "Haho" };
		msgsnd(msgqueue, &m, MSGSIZE, 0);
		kill(getppid(), SIGTERM);
		printf("child here\n");
	}
}