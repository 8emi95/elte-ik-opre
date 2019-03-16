#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //read
#include <time.h> //time
#include <string.h> //strncpy
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>

pid_t childpid;
pid_t parentpid;
int status;
key_t key;
int messageQ;
int max_szerelvenyszam;

struct message {
	long mtype;
	char mtext [2048];
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

void receivemessage( int msg, char* ret) 
{
     struct message m;
     int status; 
     status = msgrcv(msg, &m, 1024, 2, 0 ); 
     if ( status < 0 ) 
          perror("msgsnd");
	 strcpy(ret, m.mtext);
}

void handler(int signo) {
    if(signo == SIGUSR1)
	{
		printf("beertem\n");
	}
	else if(signo == SIGUSR2)
	{
		printf("bevontattak\n");
	}
}

int main(int argc, const char* argv[])
{
	pid_t parentpid = getpid();
	max_szerelvenyszam = atoi(argv[1]);
	key = ftok(argv[0],1);
	messageQ = msgget( key, 0600 | IPC_CREAT );
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
		if(r < 10)
		{
			printf("Sikertelenul vegeztem a muszakot!\n");
			kill(parentpid, SIGUSR1);
		}
		else
		{
			printf("Sikeresen vegeztem a muszakot!\n");
			kill(parentpid, SIGUSR2);
		}
		exit(1);
	}
	else
	{
		srand(time(NULL));
		int r = rand() % max_szerelvenyszam + 1;
		sendmessage(messageQ, r);
		sleep(1);
		exit(0);
	}	
	return 0;
}