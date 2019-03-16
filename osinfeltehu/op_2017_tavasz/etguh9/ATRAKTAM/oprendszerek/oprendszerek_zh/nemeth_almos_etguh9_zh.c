#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include <sys/ipc.h>

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/time.h>
#include <time.h>




struct uzenet
{
    long mtype;
    char mtext [1024]; 
};

int taxis;


void sighandler() {};



int main(int argc,char** argv)
{
	if (argc != 2)
	{
		printf("Hiba! A program pontosan egy argumentummal hívható!\n");
		exit(1);
	}
	
	printf("\nTAXI\n\n\n");
	
	srand(time(NULL));
	
			
			
			
	key_t kulcs = ftok(argv[0],1);

	int uzenetsor = msgget( kulcs, 0600 | IPC_CREAT );
	if (uzenetsor < 0)
	{ 
		perror("msgget");
		return 1;
	}
	
	
	
	signal(SIGUSR1, sighandler);

	sigset_t sig_Set;
	sigemptyset(&sig_Set);
	sigaddset(&sig_Set, SIGUSR1);
			
	sigprocmask(SIG_BLOCK, &sig_Set, NULL);


	
	int pipe_FD[2];	
	pid_t utas, taxis;
	
	if (pipe(pipe_FD) == -1)
	{
		printf("Hiba a pipe nyitaskor!\n");
		exit(1);
	}
	
	
	
	utas = fork();

	if (utas == -1)
	{
		printf("Hiba a forkolaskor!\n");
		exit(1);
	}
	if (utas == 0)
	{
		close(pipe_FD[0]);
		write(pipe_FD[1], argv[1], sizeof(argv[1]));
		close(pipe_FD[1]);
		
		kill(getppid(), SIGUSR1);
	} 
	else 
	{
		sigfillset(&sig_Set);
		sigdelset(&sig_Set, SIGUSR1);
		sigsuspend(&sig_Set);
		
		taxis = fork();
		
		if (taxis == -1)
		{
			printf("Hiba a forkolaskor!\n");
			exit(1);
		}
		if (taxis == 0)
		{
			struct uzenet uz_2;
			int status_3;
			status_3 = msgrcv(uzenetsor, &uz_2, 1024, 1, 0 );
			
			if ( status_3 < 0 )
			{
				printf("Hiba");
				exit(1);
			}
			else
			{
				printf( "Taxis: A kapott uzenet szovege: %s\n", uz_2.mtext);
			}
			
			
			
			
			
			status_3 = msgrcv(uzenetsor, &uz_2, 1024, 1, 0 );
			
			if ( status_3 < 0 )
			{
				printf("Hiba");
				exit(1);
			}
			else
			{
				printf( "Taxis: A kapott uzenet szovege: %s\n", uz_2.mtext);
			}
			
			
			
			
			uz_2.mtype = 2;
			strcpy(uz_2.mtext, "Elindultam az utasért");
	 
			status_3 = msgsnd(uzenetsor, &uz_2, sizeof(uz_2.mtext), 0 );
			
			if (status_3 < 0)
			{
				printf("Hiba");
				exit(1);
			}
		}
		else
		{
			close(pipe_FD[1]);
			
			char *lakcim = (char*)malloc(100 * sizeof(char));
			read(pipe_FD[0], lakcim, 100);
			
			printf("Taxitársaság: Kiolvasott lakcím: %s\n\n", lakcim);
			
			
			
			close(pipe_FD[0]);
			
			
			
			
			
			struct uzenet uz = {1 , "aaaaaaa"};
     
			uz.mtype = 1;
			strcpy(uz.mtext, lakcim);
	 
			int status_2;
			status_2 = msgsnd(uzenetsor, &uz, sizeof(uz.mtext), 0 );
			
			if (status_2 < 0)
			{
				printf("Hiba");
				exit(1);
			}
			
			
     
			uz.mtype = 1;
			strcpy(uz.mtext, "getpid()");
	 
			status_2;
			status_2 = msgsnd(uzenetsor, &uz, sizeof(uz.mtext), 0 );
			
			if (status_2 < 0)
			{
				printf("Hiba");
				exit(1);
			}
			
			
			
		
			
			
			
			struct uzenet uz_2;
			int status_3;
			
			status_3 = msgrcv(uzenetsor, &uz_2, 1024, 2, 0 );
			
			if ( status_3 < 0 )
			{
				printf("Hiba");
				exit(1);
			}
			else
			{
				printf( "Taxitársaság: A taxistól kapott uzenet szovege: %s\n", uz_2.mtext);
			}
			
			
			
			
			int stst;
			waitpid(utas, &stst, 0);
			waitpid(taxis, &stst, 0);
			
			
			
			int status = msgctl( uzenetsor, IPC_RMID, NULL );
			if ( status < 0 ) 
				perror("msgctl"); 
			return 0;
		}
	}
	
	return 0;
}