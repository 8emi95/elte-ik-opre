#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <wait.h>
#include <sys/ipc.h> 
#include <sys/msg.h>
/*
	A szülõ létrehoz 2 gyereket, majd ezek után bevárja a befejezésüket
*/

struct Message{
	long mtype;
	int toys;
	char msg[50];
};

int main(int argc, char* argv[]) {
	int status;
	int mq;
	key_t key;
	
	key = ftok(argv[0],2); 
	mq = msgget( key, 0600 | IPC_CREAT ); 
    if ( mq < 0 ) 
	{ 
		perror("msgget"); 
		return 1; 
    }
    
	
	int child_1 = fork();
	
	if(child_1 < 0) {
		exit(-1);
	}
	
	if(child_1 > 0) {
		// Szülõ
		int child_2 = fork();
		if(child_2 < 0) {
			printf("Fork 2");
			exit(-2);
		}
		
		if(child_2 > 0) {
			// Szülõ
			waitpid(child_1, &status, 0);
			waitpid(child_2, &status, 0);
			msgctl(mq, IPC_RMID, NULL );
		} else {
			// 2. gyerek - betörő
			sleep(1);
			struct Message msq1;
			if((msgrcv(mq, &msq1, sizeof(msq1),0,0)) < 0)
				perror("msgrcv");
			printf("Betorok: Hoppa! Elcsusztam %d jatekon!\n", msq1.toys);
			
			struct Message msq2 = {6, 0, "Csak varj, mig a kezunk koze kerulsz!\n"};
			
			if(msgsnd(mq, &msq2, sizeof(msq2), 0) < 0)
				perror("second sending");
			
			exit(0);
		}
	} else {
		// 1. gyerek - Kevin
		srand((unsigned int)time(NULL));
		int jatekok;
		jatekok = rand() % (50-20+1) + 20;
		
		struct Message message = {5, jatekok, " "};
		if ( msgsnd( mq, &message, sizeof(message) , 0 ) < 0 )
          perror("msgsnd");
          
        sleep(2);
	
		struct Message msg3;
		if(msgrcv(mq, &msg3, sizeof(msg3), 0,0) < 0)
			perror("second receive");
		
		printf("A betorok uzenete: %s", msg3.msg);
		
		exit(0);
	}
	
	return 0;
}
