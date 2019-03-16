#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <wait.h> 

struct uzenet { 
     long mtype;
     char msg [ 1024 ]; 
};

void failed_handler(int signumber)
{
  
}

void ok_handler(int signumber)
{
  exit(0);
}

void main( int argc, const char* argv[] )
{
	int uzenetsor;
	key_t kulcs; 
	kulcs = ftok(argv[0],1); 
	uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
	
	signal(SIGTERM,failed_handler);
	signal(SIGINT,ok_handler);
	
	time_t t;
	srand((unsigned) time(&t));
	
	int max = atoi( argv[1] );
	
	int pid;
	int childread[2];
	int childwrite[2];
	
	pipe(childread);
	pipe(childwrite);
	
	pid = fork();
	if (pid == 0) //child
	{
		int recieved_max = 0;
		int recieved_id = 0;
		
		struct uzenet maxuz;
		struct uzenet iduz;
		
		
		msgrcv(uzenetsor, &maxuz, strlen ( maxuz.msg ) + 1,0, 0 );
		printf("Fogad max%s:\n",maxuz.msg);
		msgrcv(uzenetsor, &iduz, strlen ( iduz.msg ) + 1, 0, 0 );
		printf("Fogad id%s:\n",iduz.msg);
		
		//printf("db utanam%s:\n",recieved_max - (int)recieved_id);
		
		int failed = rand() % recieved_max; //0 == banana, 1 == frozen
		
		if (failed <= 1)
		{
			printf("Ez a %d. szerelveny, utanam meg %ddb lesz.\n",recieved_id, recieved_max - recieved_id);
			printf("Az uzem sikertelen volt\n");
		}
		else
		{
			printf("Ez a %d. szerelveny, utanam meg %ddb lesz.\n",recieved_id, recieved_max - recieved_id);
			printf("Az uzem sikeres volt\n");
		}
		
		sleep(3);
		
		if (failed <= 1)
		{
			kill(getppid(),SIGTERM);
			msgsnd( uzenetsor, &failed, sizeof(int)+1 , 0 );
		}
		else kill(getppid(),SIGINT);
		
	}
	else //parent
	{
		int id = rand() % 10;
		int child_failed;
		
		char maxstr[1024];
		char idstr[1024];
		
		itoa(max, maxstr, 10);
		itoa(id, idstr, 10);
		/*sprintf(maxstr, "%d", max);
		sprintf(idstr, "%d", id);*/
		
		const struct uzenet maxuz = { 5, &maxstr }; 
		const struct uzenet iduz = { 5, &idstr }; 
		
		
		msgsnd( uzenetsor, &maxuz, strlen ( maxuz.msg ) + 1 , 0 );
		msgsnd( uzenetsor, &iduz, strlen ( iduz.msg ) + 1 , 0 );
		
		pause();
		
		/*msgrcv(uzenetsor, &child_failed, sizeof(uzenet)+1, 5, 0 );
		if (child_failed == 0) printf("A szerelvény elcsuszott egy bananhelyon\n");
		else printf("A szerelvény megfagyott\n");*/
	}
}