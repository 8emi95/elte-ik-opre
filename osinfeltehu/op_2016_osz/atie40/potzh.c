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

struct uzenet1
{
	long mtype;
	int ertek;
};

struct uzenet2
{
	long mtype;
	char mtext[1024];
};

int main(int argc, char** argv)
{
	srand(time(0));
	int randnum = 20 + rand() % 50;
	key_t kulcs = ftok(argv[0], 1);
	int uzenetsor = msgget(kulcs, 0600 | IPC_CREAT);
	pid_t child1, child2;
	child1 = fork();
	
	if (child1 == 0)
	{
		/*child1 Kevin*/
		sleep(1);
		int randnum = 20 + rand() % 50;
		struct uzenet1 szam = {1, randnum};
		struct uzenet2 msg;
		msgsnd(uzenetsor, &szam, sizeof(szam.ertek), 0);
		msgrcv(uzenetsor, &msg, 1024, 1, 0);
		printf("%s\n", msg.mtext);
		waitpid(child2, 0, 0);
	}
	else
	{
		child2 = fork();
		
		if (child2 == 0)
		{
			/*child2 betörõ*/
			//sleep(1);
			struct uzenet1 fog;
			struct uzenet2 msg = {1, "csak varj..."};
			msgrcv(uzenetsor, &fog, sizeof(fog.ertek), 1, 0);
			printf("%d\n", fog.ertek);
			msgsnd(uzenetsor, &msg, strlen(msg.mtext)+1,0);
			
		}
		else
		{
			/*szülõ*/
			//struct uzenet msg = {1, }
			waitpid(child1, 0, 0);
		}
		
	}
	return 0;
}