#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

struct uzenet
{
	long mtype;
	char mtext[1024];
};

int main(int argc, char** argv)
{
	/*int pipefd[2];
	pipe(pipefd);*/
	
	key_t kulcs = ftok( argv[0], 1 );
	int uzenetsor = msgget(kulcs, 0600 | IPC_CREAT);
	
	pid_t child1, child2;
	child1 = fork();
	
	char kerdes[] = "Valami?";
	
	
	if (child1 == 0)
	{
		/*child1*/
		sleep(1);
		struct uzenet fog;
		msgrcv(uzenetsor, &fog, 1024, 1, 0);
		printf("child1 Ezt kerdezte: %s\n", fog.mtext);
		sleep(2);
	}
	else
	{
		child2 = fork();
		
		if (child2 == 0)
		{
			/*child2*/
			sleep(1);
			struct uzenet fog;
			msgrcv(uzenetsor, &fog, 1024, 1, 0);
			printf("child2 Ezt kerdezte: %s\n", fog.mtext);
			sleep(3);

		}
		else
		{
			/*szülõ*/
			/*write(pipefd[1], kerdes, sizeof(kerdes));
			write(pipefd[1], kerdes, sizeof(kerdes));*/
			struct uzenet msg = {1, "Valami kerdes"};
			msgsnd(uzenetsor, &msg, strlen(msg.mtext)+1, 0);
			msgsnd(uzenetsor, &msg, strlen(msg.mtext)+1, 0);
			printf("szulo elkuldve\n");
			waitpid(child2, 0, 0);
			printf("szulo megvarta\n");
		}
	}
	
	/*close(pipefd[1]);
	close(pipefd[2]);*/

	
	return 0;
}