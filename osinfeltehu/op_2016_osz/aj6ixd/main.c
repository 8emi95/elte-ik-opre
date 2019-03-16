#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <string.h>

typedef struct Message
{
	long mtype;
	char mtext[30];
} Message;

int sendMessage(int mqid, int id)
{
	const Message m = { id, "Question???" };
	int status = msgsnd( mqid, &m, strlen(m.mtext) + 1, 0);
	if (status < 0) { perror("msgsnd hiba\r\n"); exit(1); }
	return 0;
}

int recMessage(int mqid, int id)
{
	Message m;
	int status = msgrcv(mqid, &m, 30, id, 0);
	if (status < 0) { perror("msgrcv hiba\r\n"); exit(1); }
	else
	{
		printf("%s\r\n", m.mtext);
	}
	return 0;
}

int main(int argc, char* argv[])
{
	int status;
	pid_t child;
	pid_t parent = getpid();
	
	int mqid;
	key_t key = ftok(argv[0], 1);
	mqid = msgget( key, 0600 | IPC_CREAT ); 
    if (mqid < 0) { perror("msgget"); return 1; } 
	
	int pipefd[2];
	if (pipe(pipefd) < 0) { perror("Pipe hiba!\r\n"); exit(1); }
	
	int i;
	for (i = 0; i < 2; i++) if (getpid() == parent) child = fork();
	
	if (child < 0) { perror("Fork hiba!\r\n"); exit(1); }
	if (child > 0) //parent
	{
		printf("Parent starts (%d)\n", getpid());
		
		sendMessage(mqid, 5);
		sendMessage(mqid, 5);
		
		waitpid(child, &status, 0);
		
		status = msgctl(mqid, IPC_RMID, NULL);
			if (status < 0) { perror("msgctl"); exit(1); }
		
		int ans1, ans2;
		close(pipefd[1]);
		read(pipefd[0], &ans1, sizeof(int));
		read(pipefd[0], &ans2, sizeof(int));
		close(pipefd[0]);
		
		printf("\nParent recieved: %d and %d\n", ans1, ans2);
		
		printf("Parent ended\n");
	}
	else //children
	{
		printf("Child starts (%d)\n", getpid());
		
		printf("\nChild recieved: (%d)\n", getpid());
		recMessage(mqid, 5);
		sleep(1);
		
		srand(getpid());
		int ans = rand() % 4 + 1;
		printf("\nChild sending %d...\n", ans);
		close(pipefd[0]);
		write(pipefd[1], &ans, sizeof(int));
		close(pipefd[1]);
		
		printf("Child ended (%d)\n", getpid());
	}
}