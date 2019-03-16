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

int fd[2];
char* mqname = "/msgq";
struct mq_attr attr;
mqd_t mq;
int fid;

void handler(int signumber) { }

void child_process(int i, int parentID, mqd_t mq, int* fd, char* s);

int main (int argc, char* argv[])
{
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
	strcpy(s, "");

    mq_unlink(mqname);
    mq = mq_open(mqname, O_CREAT | O_RDWR, 0600, &attr);
    if (mq < 0)
    {
        printf("Error opening message queue: %d\n", errno);
        return 1;
    }
	if (pipe(fd) == -1)
    {
        perror("Pipe failed");
        exit(1);
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
			else if (pid == 0)
			{
				child_process(i, parentID, mq, fd, s);
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
		for (i = 0; i < STUDENTS; i++) {
			db = mq_send(mq, buffer, strlen(buffer) + 1, 5);
			printf("P: Question sent: %s\t%i\n", buffer, db);
		}
		
		strcpy(buffer, "");
		pause();
		pause();
		// Receiving the answers
		for (i = 0; i < STUDENTS; ++i)
		{
			// int num;
			// if (read(fd[0], &num, 1))
			// {
				// printf("Error reading from pipe\n");
				// exit(1);
			// }
			// printf("P: Message received: %i\n", num);
			kill(pids[i], SIGUSR1);
		}
		
		for (i = 0; i < STUDENTS; ++i)
		{
			pause();
		}
		printf("Students who don't understand the question: %s\n", s);
		
		for(i = 0; i < STUDENTS; ++i)
		{
			int st;
			wait(&st);
		}
	}
	
	shmdt(s);
	// unlink("fifo.ftc");
	mq_close(mq);
	mq_unlink(mqname);
	close(fd[0]);
	close(fd[1]);
    return 0;
}

void child_process(int i, int parentID, mqd_t mq, int* fd, char* s)
{
	printf("Started\n");
	int my_pid = getpid();
	srand(my_pid);
	char buffer[MSG_LENGTH];
	int db = mq_receive(mq, buffer, MSG_LENGTH, 0);
	
	printf("CH%i: Question received: %s\n", my_pid, buffer);
	
	int randnum = rand() % 4 + 1;
	kill(parentID, SIGUSR1);
	close(fd[0]);
	 if (write(fd[1], &randnum, sizeof(randnum)))
	 {
		 printf("Error writing to pipe\n");
		 exit(1);
	 }
	 printf("CH%i: Message sent: %i\n", my_pid, randnum);
	
	pause();
	int understands = rand() % 2;
	if (!understands) {
		sprintf(buffer, "%i\t", i + 1);
		strcat(s, buffer);
		printf("Me: %s\n", buffer);
	}
	kill(parentID, SIGUSR1);
}
