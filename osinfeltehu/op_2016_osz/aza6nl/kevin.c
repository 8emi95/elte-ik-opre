#include <unistd.h>	//Fork
#include <stdio.h>	//Standard IO
#include <sys/types.h> //getpid
#include <stdlib.h> //exit

#include <string.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>
#include <sys/time.h>
#include <signal.h>   //Signalhoz kell

void handler(int signumber)
{
  if(signumber == SIGTERM)
  {
	printf( "[Kevin] Jelzést kaptam!\n");
  }
};

int main(int argc, char* argv[])
{
	
	int uzenetsor, status;
	char* mqname="/baba";
	struct mq_attr attr;
	
	mqd_t mq1,mq2;
	//MAXMSGS
	attr.mq_maxmsg=5;
	//MSGSIZE
	attr.mq_msgsize=50; 
	
	//delete if exist
	mq_unlink(mqname); 
	mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
	if ( mq1 < 0 )
	{ 
		printf("mq_open error: %d \n",errno); 
		return 1; 
	} 
	
	//Cső
	int pipefd[2];
	char message[100];
	
	if (pipe(pipefd) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	//Sigaction
	struct sigaction sigact;
	sigact.sa_handler=handler; //SIG_DFL,SIG_IGN
	sigemptyset(&sigact.sa_mask); //during execution of handler these signals will be blocked plus the signal    
	//now only the arriving signal, SIGTERM will be blocked
	sigact.sa_flags=0;
	//Szignál
	sigaction(SIGTERM,&sigact,NULL);
	
	//Alap
	int n = 2;
	pid_t childs[n];
	pid_t parent = getpid();
    printf( "[Szülő] pid: %d\n",  parent);
	
	int i;
    for ( i = 0; i < n; i++ )
	{
		pid_t pid = fork();
		if(getpid() == parent) childs[i] = pid;
        if ( pid == 0 )
        {
			if(i == 0)
			{
				
				//Üzenet
				printf( "[Kevin] pid: %d szülő: %d\n", getpid(), getppid() );
				srand(time(NULL));
				int r = 20+rand()%50;
				char uzenet[50];
				sprintf(uzenet, "%d", r);
				
				int db=mq_send(mq1,uzenet,50 , 5 ); 
				printf("[kevin]: uzenet elkuldve! %s\n", uzenet);
				sleep(3);
				
				char buffer[100];
				db=mq_receive(mq1, buffer, 50, 0);
				printf( "[Kevin] : %s\n", buffer);
				
				sigset_t sigset;
				sigfillset(&sigset);
				sigdelset(&sigset,SIGTERM);
				sigsuspend(&sigset);
				
				//Cső
				printf("[Kevin] Festéket dobok\n");
				write(pipefd[1], "festék",50);
				sleep(3);
				read(pipefd[0], message, 50);
				printf("[Kevin] Ezt olvastam csövön: %s\n", message);
				close(pipefd[0]);
				close(pipefd[1]);
				fflush(NULL);
				//Szignál
				sleep(3);
			}
			if(i == 1)
			{
				pid_t childPid = getpid();
				printf( "[Betoro] pid: %d szülő: %d\n", getpid(), getppid() );
				
				//Üzenet
				char buffer[100];
				int db=mq_receive(mq1, buffer, 50, 0);
				printf( "[Betoro] hanyadt esek: %s\n", buffer);
				db=mq_send(mq1,"Csak várj, míg a kezünk közé nem kerülsz.",50 , 5 );
				
				sleep(3);
				kill(childs[0],SIGTERM);
				
				//Cső
				read(pipefd[0], message, 50);
				printf("[Betörő] Ezt olvastam csövön: %s\n", message);
				
				char message_1[100];
				srand(time(NULL));
				if(rand()%2 == 0)
				{
					strcpy(message_1, "Na megállj csak");
				}
				else
				{
					strcpy(message_1, "Nem úszod meg szárazon!");
				}

				write(pipefd[1], message_1,50);
				close(pipefd[1]);
				close(pipefd[0]);
			}
            exit( 0 );
        }
	}
    for ( i = 0; i < n; i++ )
	{
		waitpid( childs[i] );
	}
	sleep(3);
	mq_close(mq1);
	mq_unlink(mqname);
	printf("[Szülő] leáll\n");
}

