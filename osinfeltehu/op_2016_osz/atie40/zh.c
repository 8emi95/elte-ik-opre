#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <signal.h>

struct uzenet
{
	long mtype;
	int num;
};


int main(int argc, char** argv)
{
	srand( time(0) );
	key_t kulcs = ftok( argv[0], 1 );
	int uzenetsor = msgget(kulcs, 0600 | IPC_CREAT);
	pid_t child = fork();
	int maxSzerelveny;
	maxSzerelveny = atoi(argv[1]);
	if (child < 0)
		{
			perror("fork");
			exit(1);
		}
	if (child > 0) //szülõ
	{
		int randnum = 1 + rand() % maxSzerelveny;
		struct uzenet szam = {1, randnum};
		msgsnd( uzenetsor, &szam, sizeof(szam.num), 0 );
		waitpid(child, 0, 0);
	
	}
	else //gyerek
	{
		struct uzenet fog;
		sleep(1);
		msgrcv( uzenetsor, &fog, 1024, 1, 0 );
		printf( "Szerelvenyszam: %d\nOsszes szerelveny: %d\n", fog.num, atoi(argv[1]) );

	}
	return 0;
}
