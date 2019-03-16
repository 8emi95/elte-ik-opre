#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
//#include <poll.h>
//#include <sys/shm.h>
//#include <semaphore.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

struct msg{
	long mtype;
	char mtext[256];
}

int main(int argc, char** argv){

	
	int pipefd[2];
	int gyerek1,gyerek2;
	
	key_t key = ftok(argv[0],1);
	int msgid = msgget(key,IPC_CREATE | 0666);
	
	
	gyerek1 = fork();
	
	if(gyerek1 == 0)
	{// elso gyerek
		struct msg uzenet;
		msgrcv(msgid, &uzenet,256,1,0);
		printf("Megkaptam az uzenetet: %s\n",uzenet.mtext);
		
	
	}else
	{
		gyerek2 = fork();
	
		if(gyerek2 == 0)
		{ //masodik gyerek
			struct msg uzenet;
			msgrcv(msgid, &uzenet,256,2,0);
			printf("Megkaptam az uzenetet: %s\n",uzenet.mtext);
		}else
		{ // Szulo
		
			struct msg uzenet = {1, "Mennyi 1+1?"};
			msgsnd(msgid,&uzenet,strlen(uzenet.mtext),0);
			printf("Elkuldtem a gyerekeknek a kerdest.");
		}
	
	}




}