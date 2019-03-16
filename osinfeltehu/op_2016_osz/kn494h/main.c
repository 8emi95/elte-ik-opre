#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <wait.h>
#include <time.h>
#define SIGARRIVED SIGRTMIN+1
pid_t childpid;
pid_t parentpid;
int status;
key_t key;
int messageQ;
int max_szerelvenyszam;

struct message {
	long mtype;
	char mtext [1024];
};

void sendmessage( int msg, int szerelveny )
{
	struct message m = { 2, "" };
	sprintf(m.mtext, "%ld", szerelveny);
    int status;
    status = msgsnd( msg, &m, strlen ( m.mtext ) + 1 , 0 );
    if ( status < 0 )
        perror("msgsnd");
}

void sendmessage2( int msg, char* uz )
{
	struct message m = { 2, "" };
	strcpy(m.mtext, uz);
    int status;
    status = msgsnd( msg, &m, strlen ( m.mtext ) + 1 , 0 );
    if ( status < 0 )
        perror("msgsnd");
} 

void receivemessage( int msg, char* ret) 
{
     struct message m;
     int status; 
     status = msgrcv(msg, &m, 1024, 2, 0 ); 
     if ( status < 0 ) 
          perror("msgsnd");
	 strcpy(ret, m.mtext);
}

void handler(int signumber, siginfo_t* info, void* noused) {
    printf("A vonat megerkezett: %i\n", signumber);
}

int main(int argc, const char* argv[])
{
	pid_t parentpid = getpid();
	max_szerelvenyszam = atoi(argv[1]);
	key = ftok(argv[0],1);
	messageQ = msgget( key, 0600 | IPC_CREAT );
	
	struct sigaction sigact;
	sigact.sa_sigaction = handler;
	sigact.sa_flags = SA_SIGINFO;
	sigemptyset(&sigact.sa_mask);
	sigaction(SIGARRIVED, &sigact, NULL);
	
	childpid = fork();
	if(childpid == -1)
	{
		perror("fork");
		exit(1);
	}
	if(childpid == 0)
	{
		char* uzenet;
		receivemessage(messageQ, uzenet);
		sleep(1);
		int szerelveny = atoi(uzenet);
		srand(time(NULL));
		int r = rand() % 100 + 1;
		printf("%i. szerelveny vagyok, meg %i darab lesz utanam!\n", szerelveny, (max_szerelvenyszam - szerelveny));
		sleep(1);
		if(r < 10)
		{
			printf("Sikertelenul vegeztem a muszakkal.\n");
			if(rand() % 2 < 1)
			{
				sendmessage2(messageQ, "Bananhejon elcsusztam.");		
			}
			else
			{
				sendmessage2(messageQ, "Lefagytam a zord telben.");
			}
		}
		else
		{
			printf("Sikeresen vegeztem a muszakkal.\n");
			sendmessage2(messageQ, "Beertem a garazsba!");
		}
		sleep(1);
		kill(parentpid, SIGARRIVED);
		exit(1);
	}
	else
	{
		srand(time(NULL));
		int r = rand() % max_szerelvenyszam + 1;
		sendmessage(messageQ, r);
		sleep(1);
		pause();
		char uz[1024];
		receivemessage(messageQ, uz);
		printf("Ezt kaptam a gyerektol:%s\n", uz);
		exit(0);
	}	
	return 0;
}