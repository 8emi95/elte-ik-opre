#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>



struct msgbuf
{
	long mtype;
	int osztalyzat;
};


void signal_handler(int signum)
{
        if (signum == SIGUSR1)
        {
		printf("Kaptam SIGUSR1 signal-t\n");
	}else
	printf("Kaptam vmilyen signal-t\n");
}

int main(int argc,char *argv[])
{
	signal(SIGUSR1,signal_handler);
	signal(SIGUSR2,signal_handler);
	
	int pipechild = mkfifo("/tmp/pipechild", 0600);
	if (pipechild == -1)
	{
			printf("Pipe letrehozasa nem sikerult. A program kilep. Error: %i \n", pipechild);
			unlink("/tmp/pipechild");
			exit(1);
	}
	
	int pipe = open("/tmp/pipechild", O_RDWR );
	key_t key=ftok(argv[0], 1);
	int msgq=msgget(key,IPC_CREAT | 0600);
	int nev;
	int pidchild = fork();
	if (pidchild<0)
	{
		printf("Hiba a forkolaskor\n");
		exit(1);
	}
	else if(pidchild==0) //gyerek kod
	{
		printf("gyereeek\n");
		
		read(pipe, &nev, sizeof(nev));
		printf("%d\n",nev );
		
		struct msgbuf message;
		msgrcv(msgq,&message,sizeof(message)-sizeof(long),3,0);
		printf("BOOM HEADSHOOT %d\n",message.osztalyzat );
		
		exit(0);
	}
	//szulo
	nev = 4;
	write(pipe, &nev, sizeof(nev));
	
	struct msgbuf message;
	message.osztalyzat = 5;
	message.mtype = 1;
	msgsnd(msgq, &message, sizeof(message)-sizeof(long),0);	
	
	message.osztalyzat = 6;
	message.mtype = 3;
	msgsnd(msgq, &message, sizeof(message)-sizeof(long),0);	
	printf("nemgyereeek\n");

	kill(pidchild,SIGUSR1);

	kill(pidchild,SIGUSR2);
	unlink("/tmp/pipechild");
	return 0;
}

