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
	//printf("Signal with number %i has arrived\n",signumber);
}


static key_t key;
struct message {
	long mtype;
	char mtext[1024];
};

int main(int argc, char** argv){
	setlocale(LC_ALL, "");
	
	printf("Budapest Metro Funs\n");

	//paraméter: max
	
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
		printf("msgget error\n");
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
		//printf("parent here.");
		struct message m = { 2, "" };
		sprintf(m.mtext, "%d", num);
		msgsnd(msgqueue, &m, MSGSIZE, 0);
		
		pause();
		
		sprintf(m.mtext, "%d", max);
		msgsnd(msgqueue, &m, MSGSIZE, 0);
		
		
		pause();
		msgrcv(msgqueue, &m, MSGSIZE, 2, 0);
		printf("Visszajelzes: %s --- ", m.mtext);
		msgrcv(msgqueue, &m, MSGSIZE, 2, 0);
		printf("%s", m.mtext);
		
		int status;
		waitpid(pid, &status, 0);
	//CHILD PROCESS
	} else {
		//
		struct message m;
		msgrcv(msgqueue, &m, MSGSIZE, 2, 0);
		int my_num = atoi(m.mtext);
		kill(getppid(), SIGTERM);
		
		printf("GY: Az en szamom: %d\n", my_num);
		msgrcv(msgqueue, &m, MSGSIZE, 2, 0);
		int my_max = atoi(m.mtext);
		kill(getppid(), SIGTERM);
		printf("GY: Utanam: %d\n", my_max-my_num);
		
		srand(time(NULL));
		int r = rand() % 10;
		int e = rand() % 2;
		
		if (r == 0){
			printf("GY: Elbuktam.\n");
		} else {
			printf("GY: Beertem.\n");
		}
		
		//ha elbukott
		if (r == 0){
			sprintf(m.mtext, "%s", "Bevontattak");
			msgsnd(msgqueue, &m, MSGSIZE, 0);
			if (e == 0){
				sprintf(m.mtext, "%s", "sajnos elcsusztam banahejon");
				msgsnd(msgqueue, &m, MSGSIZE, 0);
			} else {
				sprintf(m.mtext, "%s", "sajnos lefagytam a zord uton");
				msgsnd(msgqueue, &m, MSGSIZE, 0);
			}
		} else {
			sprintf(m.mtext, "%s", "Beertem");
			msgsnd(msgqueue, &m, MSGSIZE, 0);
			sprintf(m.mtext, "%s", "minden ok");
			msgsnd(msgqueue, &m, MSGSIZE, 0);
		}
		
		kill(getppid(), SIGTERM);
		
		/*struct message m;
		msgrcv(msgqueue, &m, MSGSIZE, 2, 0);
		
		printf("res: %s\n\n", m.mtext);*/
		
		/*struct message m;
		msgrcv(msgqueue, &m, MSGSIZE, 2, 0);
		printf("child: %s", m);*/
		/*struct message m = { 2, "Haho" };
		msgsnd(msgqueue, &m, MSGSIZE, 0);
		kill(getppid(), SIGTERM);
		printf("child here\n");*/
	}
	
	
	printf("\n\n");
}