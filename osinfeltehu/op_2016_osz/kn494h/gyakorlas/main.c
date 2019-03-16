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

int fd[2], nbytes;
pid_t child1pid;
pid_t child2pid;
pid_t parentpid;
char readbuffer[80];
int status;
key_t key;
int messageQ;

struct message {
	long mtype;
	char mtext [1024];
};

void sendmessage( int msg )
{
	struct message m = { 2, "Kerdes1" };
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

int main(int argc, const char* argv[])
{
	pid_t parentpid = getpid();
	key = ftok(argv[0],1);
	messageQ = msgget( key, 0600 | IPC_CREAT );
	child1pid = fork();
	if(child1pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if(child1pid == 0)
	{
		char* kerdes;
		receivemessage(messageQ, kerdes);
		printf("%s", kerdes);
		sleep(1);
		exit(1);
	}
	/*if(child2pid = fork() == -1)
	{
		perror("fork");
		exit(2);
	}
	if(child2pid == 0)
	{
		char* kerdes;
		sleep(2);
		receivemessage(messageQ, kerdes);
		sleep(1);
		printf("%s", kerdes);
		exit(2);
	}*/
	else {
	sleep(1);
	sendmessage(messageQ);
	sleep(1);
	sendmessage(messageQ);
	sleep(1);
	}	
	exit(0);
	return 0;
}