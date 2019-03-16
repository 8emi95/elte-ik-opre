#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <mqueue.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define MEMSIZE 1024


int main(){
	
	pid_t pid; //gyerek - 0 /nemgyerek 
	char bufi[100]; //buffer
	srand(time(NULL));
	
	int pipefiu[2]; //pipehoz tömb
	int pipelany[2];
	char sz[100]; //ez megy a pipeban
	
	char* mqname="/uzenetsor1";
	struct mq_attr attr;
	attr.mq_maxmsg=5; //MAXMSGS
	attr.mq_msgsize=100; //MSGSIZE 
	mqd_t mq1;

	mq_unlink(mqname); // delete if exist
	mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
     if ( mq1 < 0 ) { 
        perror("MQ hiba");
        return 1; 
    }
	
	char* mqname2="/uzenetsor2";
	mqd_t mq2;

	mq_unlink(mqname2); // delete if exist
	mq2=mq_open(mqname2, O_CREAT|O_RDWR,0600,&attr);// attr azonos
     if ( mq2 < 0 ) { 
        perror("MQ hiba");
        return 1; 
    }
	
	
	if (pipe(pipelany) == -1) {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    } 

    if (pipe(pipefiu) == -1) {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    } 
	
	pid = fork();	// ha nulla akor gyerek, negatív hiba, más szülő
   if (pid == -1) {
       perror("Fork hiba");
       exit(EXIT_FAILURE);
   }

	
	if (pid == 0) {
		// gyerek1 -fiu
		printf("Gy1\n");
		char fiunev[100];
		char uzi[100];
		read(pipefiu[0], fiunev,10);
		printf("fiu: %s\n", fiunev);
		mq_send(mq2,fiunev, strlen(fiunev), 4);
		mq_receive(mq1,uzi, 100, NULL);

	} else {
		pid_t pid2 = fork();
		if (pid2 == 0) {
			// gyerek2 -lány

			   	char lanynev[100];
				char lanyuzi[100];
				read(pipelany[0], lanynev,10);
				printf("lany: %s\n", lanynev);
				mq_send(mq1,"viiii", 5, 4);
				mq_receive(mq1,lanyuzi, 100, NULL);
			
			} else {
				// szuloooo
				printf("Szulo var...\n");
				write(pipelany[1], "julcsi", 6);
				write(pipefiu[1], "aron", 4);
				waitpid(pid, NULL, 0);
				waitpid(pid2, NULL, 0);
				printf("Szulo vege\n");
				//shmctl(oszt_mem_id,IPC_RMID,NULL);
			}
		}
	
	
	
	
	return 0;
}