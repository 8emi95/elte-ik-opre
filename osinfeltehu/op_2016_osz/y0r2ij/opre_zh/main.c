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

struct Question {
	int sorszam;
	int max_szerelveny;
	//int tuleltem_e;
	char uzenet[10];
};

void handler(int signumber){
	if(signumber == SIGUSR1){}
}

int mqSend(int mq, struct Question qu)
{ 
    int status;
    status = msgsnd( mq, &qu, sizeof(qu) , 0 );
    if ( status < 0 )
        perror("msgsnd");
    return 0;
} 

int mqReceive(int mq, struct Question* qu) 
{ 
     int status; 
     status = msgrcv(mq, qu, sizeof(*qu), 0, 0 ); 
	// printf("%d %d\n",qu->sorszam, qu->max_szerelveny);
     if ( status < 0 ) 
          perror("msgrcv"); 
	 return 0;
}

int main(int argc, char* argv[])
{
	signal(SIGUSR1, handler);
	srand(time(0));
	int max_szerelveny = atol(argv[1]);
	
    int messageQueue, status; 
    key_t key;
	
	key = ftok(argv[0],2); 
	messageQueue = msgget( key, 0600 | IPC_CREAT ); 
    if ( messageQueue < 0 ) 
	{ 
		perror("msgget"); 
		return 1; 
    } 
	
	
	pid_t child = fork(); 
	
	if (child < 0)
	{
		perror("fork"); 
        return 1; 
	}
	else if(child > 0) // parent
	{
		int sorszam = (rand() % max_szerelveny) +1;
		struct Question qu = { sorszam, max_szerelveny, 0 };
		mqSend(messageQueue, qu);
		int status;
		printf("Szulo: Varom a jelzest.\n");
		pause();
		
		mqReceive(messageQueue, &qu);
		printf("Szulo: a gyermekem sorsa: %s\n", qu.uzenet);
		
		waitpid(child,&status,0); 
		status = msgctl( messageQueue, IPC_RMID, NULL ); 
		if ( status < 0 ) 
			perror("msgctl");
		
		wait(NULL);
	}
	else // child
	{
		struct Question qu;
		mqReceive(messageQueue, &qu);
		if(rand() % 10 >= 1)
		{
			printf("Gyerek: Ugyes kis metroszerelveny vagyok!\nGyerek: En a %d. szerelveny vagyok es meg %d szerelveny varhato.\n\n", qu.sorszam, (qu.max_szerelveny-qu.sorszam));
			strcpy(qu.uzenet, "Jo!\n");
		}
		else
		{
			printf("Gyerek: Kinyiffantam!\nGyerek: En a %d. szerelveny vagyok es meg %d szerelveny varhato.\n\n", qu.sorszam, qu.max_szerelveny);
			strcpy(qu.uzenet, "Rossz!\n");
		}
		
		sleep(1);
		printf("Gyerek: most jon a kill, mar sleepeltem.\n");
		kill(getppid(), SIGUSR1);
		
		mqSend(messageQueue, qu);
	}
	
	wait(NULL);
	
}