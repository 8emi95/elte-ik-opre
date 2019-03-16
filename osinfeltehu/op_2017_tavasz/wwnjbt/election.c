// linekeléshez: -lrt

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>
#include <sys/shm.h>
#include <time.h>
#include <signal.h>
#define STUDENTS 2
#define MSG_LENGTH 500

int pipefd[2];
char* mqname = "/asdfasdf";
char* mqname2 = "/asdfasdfsdf";
struct mq_attr attr;
mqd_t mq, mq2;

void handler(int signumber) { }

int main (int argc, char* argv[])
{
	srand(time(NULL));
	signal(SIGUSR1, handler);
    pid_t parentID = getpid();
	pid_t pid, pids[STUDENTS];
    int status;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = MSG_LENGTH;
	
    key_t k = ftok(argv[0],1);
    int shrd_mem_id = shmget(k,500,IPC_CREAT|S_IRUSR|S_IWUSR);
    char *s;
	s = shmat(shrd_mem_id,NULL,0);

    mq_unlink(mqname);
    mq = mq_open(mqname, O_CREAT | O_RDWR, 0600, &attr);
    mq2 = mq_open(mqname2, O_CREAT | O_RDWR, 0600, &attr);
    if (mq < 0)
    {
        printf("Error opening message queue: %d\n", errno);
        return 1;
    }
	if (mq2 < 0)
    {
        printf("Error opening message queue: %d\n", errno);
        return 1;
    }
	
	int i;
	for(i = 0; i < STUDENTS; ++i)
	{
		if(parentID == getpid())
		{
			pid = fork();
			if(pid < 0)
			{
				printf("fork fail (%d)", i);
				return 1;
			}
			else
				pids[i] = pid;
		}
	}
	if (pid > 0) // Prof
	{
		printf("Parent started\n");
		int i;
		int db;
		char buffer[MSG_LENGTH] = "Are you all here?";
		for (i = 0; i < STUDENTS + 1; i++) {
			db = mq_send(mq, buffer, strlen(buffer) + 1, 5);
		}
		printf("P: Question sent: %s\t%i\n", buffer, db);
		
		pause();
		pause();
		// Receiving the answers
		printf("Hey");
		for (i = 0; i < STUDENTS; ++i)
		{
			strcpy(buffer, "");
			int db = mq_receive(mq2, buffer, 1, NULL);
			printf("P: Message received: %s\t%i\n", buffer, db);
			kill(pids[i], SIGUSR1);
		}
		// printf("Students who don't understand the question: %s", s);
		
		for(i = 0; i < STUDENTS; ++i)
		{
			int st;
			wait(&st);
		}
		shmdt(s);
	}
	else
	{
		printf("Started\n");
		char buffer[MSG_LENGTH];
		int db = mq_receive(mq, buffer, MSG_LENGTH, 0);
		
		int my_pid = getpid();
		
		printf("CH%i: Question received: %s\n", my_pid, buffer);
		kill(parentID, SIGUSR1);
		
		int randnum = rand() % 4 + 1;
		sprintf(buffer, "%i", randnum);
		db = mq_send(mq2, buffer, strlen(buffer) + 1, 5);
		printf("CH%i: Message sent: %s\t%i\n", my_pid, buffer, db);
		
		pause();
		// int understands = rand() % 2;
		// if (!understands) {
			// sprintf(buffer, "%i", my_pid);
			// strcpy(s, buffer);
		// }
		// shmdt(s);
	}
	
	mq_close(mq);
	mq_unlink(mqname);
    return 0;
}