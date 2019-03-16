#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 
#include <signal.h>
#include <sys/wait.h>

#define KEY 2003


struct msg{
	long mtype;
	char mtext[13];
} ;

int main(int argc, char **argv) {
	pid_t child = fork();
	int status;
	int azon = msgget((key_t) KEY, IPC_CREAT | 0666);
	if(child > 0) {
		printf("%s", "szulo");
		struct msg uzenet;
		uzenet.mtype = 100;
		char *uzen = "probauzenet";


		strcpy(uzenet.mtext,uzen);
		if(msgsnd(azon, &uzenet, strlen(uzen), 0)<0)
			printf("%s", "msgsnd error");
		waitpid(child, &status,0);
	} else {
		printf("%s","gyerek");
		struct msg uzenet;
		azon = msgget((key_t) KEY, 0);
		if(msgrcv(azon, &uzenet, 25, long(100), IPC_NOWAIT | MSG_NOERROR)<0)
			printf("%s", "msgrcv error");
		else
			printf("%s", uzenet.mtext);
	}
}
