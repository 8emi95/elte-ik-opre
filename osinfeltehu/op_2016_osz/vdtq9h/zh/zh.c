#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

typedef struct msg
{
    long mtype;
    char message[100];
}Msg;



int main(int argc, char* argv[])
{
	pid_t kevin,betoro;
	srand(time(0));
	
	kevin = fork();
	if(kevin > 0)	// szülő
	{
		betoro = fork();
		if(betoro > 0)	// szulő
		{
			
		}
		else	// kevin
		{
		int rand_number = (rand()%30)+21;
		key_t queue_key = ftok(argv[0],1);
		int queue = msgget(queue_key,0600 | IPC_CREAT  );
		
		Msg success;
		success.mtype = 1;
		sprintf(success.message,"%d",rand_number);
		int msg_status = msgsnd(queue,&success,sizeof(success),0);
		
		Msg received;
		received.mtype = 1;
		
		msg_status = msgrcv(queue, &received, sizeof(received), 0, MSG_NOERROR | IPC_NOWAIT);
		printf("%s\n",received.message);
		}
	}
	else	// betoro
	{

		
		
			key_t queue_key = ftok(argv[0],1);
			int queue = msgget(queue_key,0600 | IPC_CREAT  );
			
			Msg received;
			received.mtype = 1;
			int msg_status = msgrcv(queue, &received, sizeof(received), 0, MSG_NOERROR | IPC_NOWAIT);
			printf("Betoro hanyatt esett\n");
			Msg send;
			send.mtype = 1;
			strcpy(send.message,"csak varj amig kezeink koze nem kerulsz!");
			
			msg_status = msgsnd(queue,&send,sizeof(send),0);
			
			exit(0);
	}
	
	waitpid(kevin);
	waitpid(betoro);

}	