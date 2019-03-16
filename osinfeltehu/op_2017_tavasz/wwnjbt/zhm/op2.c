#include <stdio.h>
#include <unistd.h> //fork
#include <sys/wait.h> //waitpid
#include <stdlib.h> //exit
#include <string.h> //strlen
#include <sys/time.h> //time
#include <signal.h> //sigval
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#define STUDENTS 2
#define MSG_LENGTH 50
	
int gyerekek = 2;
int kezdo;
	
void handler(int sign){
	printf("\n");
	if(sign==SIGUSR2);
}

int main(){
	
	char* mqname = "/baba1";
	struct mq_attr attr;
	mqd_t mq;
	attr.mq_maxmsg=5; //MAXMSGS
	attr.mq_msgsize=50; //MSGSIZE 
	
	pid_t parentID = getpid();
	pid_t pid, pids[gyerekek];
	int pp[2],i;
	char str[50];
		
	signal(SIGPOLL, handler);
	signal(SIGTERM, handler);
	signal(SIGUSR1,handler);
	
	mq_unlink(mqname);
    mq = mq_open(mqname, O_CREAT | O_RDWR, 0600, &attr);
	if (mq < 0)
    {
        printf("Error opening message queue: %d\n", errno);
        return 1;
    }
	
	for(i = 0; i < gyerekek; ++i){
		if(parentID == getpid()){
			pid=fork();
			if(pid < 0) {
				printf("fork fail (%d)",i);
				return 1;
			}
			pids[i]=pid;
		}
	}
	if(pid > 0) { // Parent
		srand(time(NULL));
		kezdo = rand()%2;
		
				
		for(i = 0; i < gyerekek; ++i){
			pause();
			printf("Megkaptam egyik jelzest..\n");
		}
		
		for(i = 0; i < gyerekek; ++i){
			char buffer[50] = "Are you all here?";
			if (mq_send(mq, buffer, sizeof(buffer), 5))
			{
				printf("Error sending message: %d\n", errno);
				return 1;
			}
			kill(pids[i],SIGUSR1);
			sleep(1);
		}
		
		
		for(i = 0; i < gyerekek; ++i){
			int st;
			wait(&st);
		}

		printf("Vege az oranak!\n");
		mq_close(mq);
		mq_unlink(mqname);
	}
	else { // Child
		
		printf("  Itt vagyok, jelzek vissza (%d)\n",getpid());
		kill(getppid(),SIGUSR1);
		
		printf("\n  Most varok a kezdesre..(%d)\n",getpid());
		pause();
		
		char buffer[MSG_LENGTH];
		mq_receive(mq, buffer, MSG_LENGTH, 0);
		
		printf("CH%d: A kapott uzenet: %s\n",getpid(), buffer);
		mq_close(mq);
	}
	
	return 0;
}