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

void handler(int signumber){
  if (signumber == SIGUSR1)
  {
	  
  };
}

struct message { 
     long mtype;
	 int index;
	 int sum;
	 char text[100];
}; 

int send(int mq, struct message mes)
{ 
     int status;
     status = msgsnd( mq, &mes, sizeof(mes) , 0 );
     if ( status < 0 )
          perror("msgsnd");
     return 0;
} 

int receive(int mq, struct message* mes) 
{ 
     int status; 
     status = msgrcv(mq, mes, sizeof(*mes), 0, 0 ); 
     if ( status < 0 ) 
          perror("msgrcv"); 
	 return 0;
} 

int main(int argc, char* argv[])
{
	signal(SIGUSR1,handler);
	
	srand(time(0));
	
	int messageQueue, status; 
    key_t key;
	
	key = ftok(argv[0],2); 
	messageQueue = msgget( key, 0600 | IPC_CREAT ); 
	
	
	
	if ( messageQueue < 0 ) 
	{ 
		perror("msgget"); 
		return 1; 
    } 
	
	int szam = atoi(argv[1]);
	int i = 1;
	int is_parent = 0;
	while (i++ < szam && is_parent == 0)
	{
		pid_t child = fork(); 
	
		if (child < 0)
		{
			perror("fork"); 
			return 1; 
		}
		else if(child > 0) //parent
		{
			struct message m;
			m.sum = szam;
			m.index = i;
			
			send(messageQueue, m);
			int status;
		
			pause();
		
			receive(messageQueue, &m);
			printf(" A szerelvenytol kapott uzenet: %s\n", m.text);
		
			wait(NULL); 
		
			status = msgctl( messageQueue, IPC_RMID, NULL ); 
			if ( status < 0 ) 
				perror("msgctl"); 
		
		}
		else //child
		{
			struct message mes;
			receive(messageQueue, &mes);
		 
			int success = rand() % 10;
		
			if (success != 5)
			{
				printf("En a %d .szerelveny vagyok es osszesen %d szerelveny van es sikerrel teljesitettem.\n", mes.index, mes.sum);
				strcpy(mes.text,"Jo");
			}
			else
			{
				printf("En a %d .szerelveny vagyok es osszesen %d szerelveny van es nem sikerult teljesitenem.\n", mes.index, mes.sum);
				strcpy(mes.text,"Rossz");
			}
		
			kill(getppid(),SIGUSR1);
		
			send(messageQueue, mes);
			exit();
		}
  }
	
	
	
	
}