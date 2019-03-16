#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
#include <wait.h>
#include <time.h>

struct uzenet {
	long mtype;
	char mtext[ 1024 ];
};

int kuld( int uzenetsor, struct uzenet uz ) {
	int status;
	
	status = msgsnd( uzenetsor, &uz, strlen(uz.mtext) + 1, 0 );
	
	if(status < 0) {
		perror("msgsnd");
	}
	return 0;
}

int fogad( int uzenetsor, int mtype, char mtext[1024] ) {
	struct uzenet uz;
	int status;

	status = msgrcv( uzenetsor, &uz, 1024, mtype, 0 );
	if(status < 0)
		perror("msgrcv");
	else
		strcpy(mtext, uz.mtext);
	return 0;
}

void handler(int signum) {
}

int main(int argc, char *argv[]) {
	pid_t child;
	char buf[100];
	int uzenetsor, status, i;
	key_t kulcs;

	kulcs = ftok(argv[0], 1);
	uzenetsor = msgget( kulcs, 0600 | IPC_CREAT );
	if(uzenetsor < 0) {
		perror("msgget");
		exit(1);
	}

	signal(SIGUSR1, handler);

	i = 0;
	do {
		child = fork();
		i++;
	} while(child > 0 && i < atoi(argv[1]));

	if(child < 0) {
		perror("Fork error");
		exit(1);
	}

	if (child > 0) { // parent
		int max = atoi(argv[1]);
		struct uzenet uz = { 0, "" };
		char mtext[1024];
		for(i = 1; i <= max; i++) {
			uz.mtype = i;
			sprintf( uz.mtext, "%d", i );
			kuld(uzenetsor, uz);
			sprintf( uz.mtext, "%d", max );
			kuld(uzenetsor, uz);
		}
		wait(NULL);
		for(i = 1; i <= max; i++) {
			fogad(uzenetsor, i, mtext);
			printf("%s", mtext);
		}
	} else {
		char mtext[1024];
		struct uzenet uz = {i, ""};
		int id, max, r;
		fogad(uzenetsor, i, mtext);
		printf("En vagyok a %s. szerelveny.\n", mtext);
		id = atoi(mtext);
		fogad(uzenetsor, i, mtext);
		max = atoi(mtext);
		printf("Meg %d szerelveny jon utanam.\n", max - id);
		srand(time(NULL));
		r = rand() % 100;
		if(r < 10) {
			printf("Sajnos nem sikerult az uzem... :(\n");
			if(r < 5) {
				strcpy(uz.mtext, "Elcsusztam egy bananhejon.\n");
			} else {
				strcpy(uz.mtext, "Lefagytam a zord telben.\n");
			}
		} else {
			printf("Ez az! Sikerult az uzem! :)\n");
			strcpy(uz.mtext, "Sikeresen beertem.\n");
		}
		kill(getppid(), SIGUSR1);
		kuld(uzenetsor, uz);
	}
}
