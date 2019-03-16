#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>		//fork
#include <sys/wait.h>	//waitpid
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 

struct message { 
     long mtype;
     char mtext[1024]; 
};

int main(int argc, char** argv)
{
	printf("\n*********************** Fuvarszervezo ***********************\n\n");

	int childParentPipe[2];		//Gyerek írja
	pipe(childParentPipe);
	srand(time(NULL));
	
	
	//Utas létrehozása
	pid_t passangerPid = fork();
	if (passangerPid<0){perror("A fork hivas sikertelen volt!\n"); exit(1);}
	
	if (passangerPid > 0)			//Szülõ
	{
		char* receivedAddress;
		read(childParentPipe[0],receivedAddress,strlen(argv[1])+1);
		printf("Az utas cime: %s\n\n",receivedAddress);
		
		
		//Taxis létrehozása
		int messageQueue;
		key_t key;
		key = ftok(argv[0],1);
		messageQueue = msgget( key, 0600 | IPC_CREAT ); 
		if (messageQueue < 0 ) { perror("msgget"); return 1;} 
			
		pid_t taxiDriverPid = fork();
		if (taxiDriverPid<0){perror("A fork hivas sikertelen volt!\n"); exit(1);}
		
		if (taxiDriverPid> 0)			//Szülõ
		{
			struct message addressMessage;
			addressMessage.mtype = 1;
			strcpy(addressMessage.mtext,receivedAddress);
			const struct message phoneNumberMessage = { 1, passangerPid };
			
			int status;
			status = msgsnd( messageQueue, &addressMessage, strlen ( addressMessage.mtext ) + 1 , 0 );
			if ( status < 0 ) perror("msgsnd");
			status = msgsnd( messageQueue, &phoneNumberMessage, strlen ( phoneNumberMessage.mtext ) + 1 , 0 );
			if ( status < 0 ) perror("msgsnd");
			
			waitpid(passangerPid,NULL,0);
			waitpid(taxiDriverPid,NULL,0);
			msgctl( messageQueue, IPC_RMID, NULL ); 
		}
		else							//Taxis
		{
			struct message addressMessage, phoneNumberMessage;
			struct message reply = {1,"A taxis elindult"};
			int status;
			status = msgrcv(messageQueue, &addressMessage, 1024, 1, 0);
			if ( status < 0 ) perror("msgrcv");
			status = msgrcv(messageQueue, &phoneNumberMessage, 1024, 1, 0);
			if ( status < 0 ) perror("msgrcv");
			printf("Taxis altal kapott adatok:\n Lakcim: %s\n Telefonszam: %d\n\n",addressMessage.mtext,phoneNumberMessage.mtext);
			msgsnd( messageQueue, &reply, strlen ( reply.mtext ) + 1 , 0 );
		}
	}
	else						//Utas
	{
		close(childParentPipe[0]);
		char* address = argv[1];
		write(childParentPipe[1],address,strlen(argv[1])+1);
	}

	return 0;
}