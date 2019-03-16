#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <mqueue.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sched.h>
#include <signal.h>
#include <bits/siginfo.h>
#include <sys/ipc.h> 
#include <sys/msg.h>

int main(int argc, char *argv[]){

	int pipefd[2]; // unnamed pipe file descriptor array
	pid_t pid;
	char cim[128];  // char array for reading from pipe

	if(argc != 2){
		printf("Add meg a címet argumentumként\n");
		return 1;
	}
	if(sizeof(argv[1]) > sizeof(cim)){
		printf("Túl hosszú cím\n");
		return 1;
	}
	if (pipe(pipefd) == -1){
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	pid = fork();	// creating parent-child processes
	if (pid == -1) {
		perror("Fork hiba");
		exit(EXIT_FAILURE);
	}

	if (pid == 0){	//utas
		int i;
		for(i = 0; i < sizeof(argv[1])/sizeof(char); i++){
			cim[i] = argv[1][i];
		}
		close(pipefd[0]);
		write(pipefd[1], cim,sizeof(cim));
		close(pipefd[1]);
		fflush(NULL);
	} 
	else{   //kozpont
		pid_t utas = pid;
		sleep(1);
		close(pipefd[1]);
		read(pipefd[0],cim,sizeof(cim));
		printf("Gyerek olvasta uzenet: %s \n",cim);
		close(pipefd[0]);

		
		pid_t taxi; 
		int uzenetsor, status;
		char* mqname1="/tel1";
		char* mqname2="/tel2";
		struct mq_attr attr;
		mqd_t mq1,mq2;
		attr.mq_maxmsg=5; //MAXMSGS
		attr.mq_msgsize=50; //MSGSIZE 
		//
		mq_unlink(mqname1); // delete if exist
		mq1=mq_open(mqname1, O_CREAT|O_RDWR,0600,&attr);
		mq2=mq_open(mqname2, O_CREAT|O_RDWR,0600,&attr);
		if ( mq1 < 0 || mq2 < 0) { 
			printf("mq_open error: %d \n",errno); 
			return 1; 
		} 
		char buffer[100];	// mq_send, mq_receive uses char array
		//printf("Buffer length: %d\n",strlen(buffer));
		taxi = fork(); 
		if ( taxi > 0 ) { //kp
			char uzenet[30];//="PID";
			uzenet[0] = utas;
			int db=mq_send(mq1,uzenet,1, 5 );  //Parent sends a message. 
			//sleep(1); // ha ez nincs a fogad be is olvassa!!!!
			//mq_send(mq1,"Hajra Debrecen!",20, 4);
			//mq_receive(mq1, buffer, strlen(buffer), 0); // get the first message
			//printf("Szulo: uzenet elkuldve!%d\n",db);
			//wait( NULL ); 
			sleep(0);
			// After terminating child process, the message queue is deleted. 
			mq_close(mq1);
			mq_unlink(mqname1);
			int db2=mq_receive(mq2,buffer, 50, 0);
			printf("taxi uzeni: %s\n", buffer);
			mq_close(mq2);
				//printf("Szulo vege!\n"); 
		} else  { // taxi
		//sleep(1); 
		//mq2=mq_open(mqname,O_RDWR);
		//strcpy(buffer,"                  ");

		//int db=mq_receive(mq1,buffer, 54,NULL);
		// 5 is the message priority
		//
		//printf("Buffer length: %d\n",strlen(buffer));
		//printf("mq_receive : %s, olvasott bájtok hossza: %d errno: %s\n",buffer, db,strerror(errno));
		int db=mq_receive(mq1,buffer, 50, 0);
		printf("taxi: %d\n", buffer[0]);
		mq_close(mq1);
		char uzenet[30]="Megyek";
			int db2=mq_send(mq2,uzenet, 7, 5 );  //Parent sends a message. 
			//sleep(1); // ha ez nincs a fogad be is olvassa!!!!
			//mq_send(mq1,"Hajra Debrecen!",20, 4);
			//mq_receive(mq1, buffer, strlen(buffer), 0); // get the first message
			//printf("Szulo: uzenet elkuldve!%d\n",db);
			//wait( NULL ); 
			sleep(0);
			// After terminating child process, the message queue is deleted. 
			mq_close(mq2);
			mq_unlink(mqname2);
		//mq_send(mq1,"Gyerek befejezte!", 20,6);
		//msgsnd(uzenetsor,&valasz,sizeof(struct uzenet),0);
		return 0; 
		  // The child process receives a message. 
		}
		
	
		waitpid(utas,&status,0);
	}
	exit(EXIT_SUCCESS);	// force exit, not necessary
}