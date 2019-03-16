#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <regex.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf
{
	char szoveg [50];
};

//Childosdi
void kill_children();
void run1();
void run2();
void signal_handler(int signum);
void wait_children();

int pchilds;
pid_t pidchilds [2];
int msgq;
int mehet;

void signal_handler(int signum)
{
	printf("Parent: Signal got... \n");
	mehet=mehet+1;
}

void wait_children()
{
	mehet=0;
	while (mehet<2)
	{
		//SKIP
		sleep(1);
	}
}

void run1()
{
	printf("1.PID:%d \n" ,getpid());
	kill(getppid(), SIGINT);
	printf("1: keszen allok a munkara jelzes kuldve az oktatonak\n");
	struct msgbuf buf;
	msgrcv(msgq,&buf,sizeof(buf)-sizeof(long),0,0);
	printf("A kapott kerdes: %i\n\n\n", buf.szoveg);
	while (1)
	{
		sleep(1);
	}
}

void run2()
{
	printf("2.PID:%d \n" ,getpid());
	kill(getppid(), SIGINT);
	printf("2: keszen allok a munkara jelzes kuldve az oktatonak\n");
	struct msgbuf buf;
	msgrcv(msgq,&buf,sizeof(buf)-sizeof(long),0,0);
	printf("A kapott kerdes: %i\n\n\n", buf.szoveg);
	while (1)
	{
		sleep(1);
	}
	
}

int main(int argc, char * argv[])
{
	printf("szuloPID:%d \n" ,getpid());
	signal(SIGINT, signal_handler);
	srand(time (NULL));
	int pipechild = mkfifo("pipechild", 0600);
	if (pipechild == -1)
	{
		printf("Pipe letrehozasa nem sikerult. A program kilep.");
		unlink("pipechild");
		exit(1);
	}
	pchilds = open("pipechild", O_RDWR );

	key_t key=ftok(argv[0], 1);
    msgq=msgget(key,IPC_CREAT | 0600);
	pidchilds[0] = fork();
	if (pidchilds[0]<0)
	{
		printf("Hiba a forkolaskor\n");
		return 1;
	}
	else if(pidchilds[0]==0)
	{
		run1();
	}

	pidchilds[1] = fork();
	if (pidchilds[1]<0)
	{
		printf("Hiba a forkolaskor\n");
		return 1;
	}
	else if(pidchilds[1]==0)
	{
		run2();
	}
	printf("Oktato var a hallgatokra\n");	
	wait_children();
	printf("Oktato: hallgatok jelentkeztek\n");
	srand(time(NULL));
	struct msgbuf buf;
	buf.szoveg="kerdes";
	msgsnd(msgq,&buf,sizeof(buf)-sizeof(long),0);
	kill_children();
	unlink("pipechild");	
    return 0;
}


void kill_children()
{
	printf("\n## Childok leallitasa ##\n");

	int statu;
	kill(pidchilds[0], SIGTERM);
	printf("\n\t1. child leallitasa...");
	wait(&statu);
	printf("Done\n");
	kill(pidchilds[1], SIGTERM);
	printf("\t2. child leallitasa...");
	wait(&statu);
	printf("Done\n");
	printf("\n!!  Childok lealltak  !!\n");
}
