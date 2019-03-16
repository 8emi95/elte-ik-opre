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
};

int main(int argc, char **argv) {
	pid_t child = fork();
	int status;
	int stat;
	int azon = msgget((key_t) KEY, IPC_CREAT | 0666);
	int status2;
	if(child > 0) {
		printf("%s\n", "szulo");
		struct msg uzenet;
		uzenet.mtype = 100;
		char* uzen = "probauzenet\n";
		strcpy(uzenet.mtext,uzen);
		if(msgsnd(azon, &uzenet, strlen(uzen), 0)<0)
			printf("%s", "msgsnd error\n");
		struct msg vmi;
		if(msgrcv(azon, &vmi, 25, (long)200, 0)<0)
			printf("%s", "msgrcv error\n");
		else
			printf("%s", vmi.mtext);
		if ( stat < 0 ) 
               perror("msgctl");
		pid_t child2 = fork();
		if(child2 > 0) {
			struct msg uzenet;
			uzenet.mtype = 300;
			char* uzen = "probauzenet\n";
			strcpy(uzenet.mtext,uzen);
			if(msgsnd(azon, &uzenet, strlen(uzen), 0)<0)
				printf("%s", "msgsnd error\n");
		} else {
			struct msg uzenet;
			//azon = msgget((key_t) KEY, 0);
			if(msgrcv(azon, &uzenet, 25, (long)300, 0)<0)
				printf("%s", "msgrcv error\n");
			else
				printf("%s", uzenet.mtext);
		}
		waitpid(child, &status,0);
		waitpid(child2, &status2,0);
		stat = msgctl( azon, IPC_RMID, NULL );
        return 0;

	} else {
		printf("%s","gyerek\n");
		struct msg uzenet;
		azon = msgget((key_t) KEY, 0);
		if(msgrcv(azon, &uzenet, 25, (long)100, 0)<0)
			printf("%s", "msgrcv error\n");
		else
			printf("%s", uzenet.mtext);
		struct msg oth;
		oth.mtype = 200;
		char* szoveg = "szoveg\n";
		strcpy(oth.mtext,szoveg);
		if(msgsnd(azon, &oth, strlen(szoveg), 0)<0)
			printf("%s", "msgsnd error\n");
		printf("%s","Uzenet elkuldve\n");
		return 1;
	}
}
