#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sched.h>
#include <signal.h>
#include <bits/siginfo.h>

#define MSG_SIZE 64
void handler(int signum) {
	//printf("Signal arrived! \n");
}

int main(int argc, char *argv[]) {
	char* mqname = "AwesomeMessageQue";
	struct mq_attr attr;
	attr.mq_maxmsg = 10;
    attr.mq_msgsize = MSG_SIZE;
	mq_unlink(mqname);
	
	int max;
	if(argc <= 1){
		printf("Nincs megadva max!");
		exit(1);
	}
	max = atoi(argv[1]);
	srand((unsigned) time(NULL));
	
	pid_t pid;
	pid = fork();
	if(pid < 0) {
		perror("Hiba a forknál!");
		exit(1);
	}
	if(pid == 0) {
		//child
		//printf("debug: signal elott\n");
		char buffer[100];
		signal(SIGUSR1,handler);
		//printf("debug: signal utan\n");
		pause();
		//printf("debug: pause utan\n");
		mqd_t mq = mq_open(mqname,O_CREAT|O_RDWR,0600,&attr);
		int ossze_szerelveny;
		char* recvmsg;
		int en_szamom;
		//mq_receive(mq,&ossze_szerelveny,sizeof(int),0);
		mq_receive(mq,recvmsg,MSG_SIZE,0);
		//memcpy(&en_szamom,recvmsg,sizeof(int));
		//en_szamom=atoi(recvmsg);
		//en_szamom = *((int *)recvmsg);
		printf("En szamom: %s\n",recvmsg);
		mq_close(mq);
		
		
	}else {
		//parrent
		int szerelvenyszam = (rand() % max) + 1;
		mqd_t mq = mq_open(mqname,O_CREAT|O_RDWR,0600,&attr);
		//char msg[MSG_SIZE];
		//snprintf(msg,MSG_SIZE,"%i",szerelvenyszam);
		//memcpy(msg,&szerelvenyszam,sizeof(int));
		printf("MSG: %i\n",szerelvenyszam);
		mq_send(mq,"hehe",MSG_SIZE,0);
		mq_close(mq);
		//printf("debug: send utan\n");
		sleep(1);
		kill(pid,SIGUSR1);
		//printf("debug: kill utan\n");

	}
	
	
	
	return 0;
}