#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <time.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <errno.h>




struct uzenet
{
    long mtype;
    char mtext[1024];
};




int parameter_Read_From_Signal;




void sighandler() {};




void handler(int signumber, siginfo_t* info, void* nonused)
{
	switch (info->si_code)
	{
		case SI_QUEUE:			
			parameter_Read_From_Signal = info->si_value.sival_int;
			break;
		default:
			printf("HIBA  -  handler\n");
	} 
}




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
		printf("HIBA  -  msgget\n");
		exit(1);
	}
	
	
	
	
	signal(SIGUSR1, sighandler);

	sigset_t sig_Set;
	sigemptyset(&sig_Set);
	sigaddset(&sig_Set, SIGUSR1);
			
	sigprocmask(SIG_BLOCK, &sig_Set, NULL);


	
	
	int pipe_FD[2];
	
	if (pipe(pipe_FD) == -1)
	{
		printf("HIBA  -  pipe\n");
		exit(1);
	}
	
	
	
	
	pid_t utas, taxis;
	
	utas = fork();

	if (utas == -1)
	{
		printf("HIBA  -  1. fork\n");
		exit(1);
	}
	if (utas == 0)
	{
		
//*********************** UTAS ***********************
		
		close(pipe_FD[0]);
		write(pipe_FD[1], argv[1], 1+strlen(argv[1]));
		close(pipe_FD[1]);
		
		kill(getppid(), SIGUSR1);
		
		
		
		
		struct sigaction sigact;
		sigact.sa_sigaction = handler; //instead of sa_handler, we use the 3 parameter version
		sigemptyset(&sigact.sa_mask); 
		sigact.sa_flags = SA_SIGINFO; //we need to set the siginfo flag 
		sigaction(SIGUSR1, &sigact, NULL);
			
			
		sigfillset(&sig_Set);
		sigdelset(&sig_Set, SIGUSR1);
		sigsuspend(&sig_Set);
		
		printf("almalmalmalmalma  %i  \n", parameter_Read_From_Signal);
		
//*********************** UTAS VÉGE ***********************
	
	}
	else 
	{
		sigfillset(&sig_Set);
		sigdelset(&sig_Set, SIGUSR1);
		sigsuspend(&sig_Set);
		
		taxis = fork();
		
		if (taxis == -1)
		{
			printf("HIBA  -  2. fork\n");
			exit(1);
		}
		if (taxis == 0)
		{
			
//*********************** TAXIS ***********************
			
			struct uzenet uz;
			int status = msgrcv(uzenetsor, &uz, 1024, 1, 0);   //ha itt pl. 1023 lenne -> HIBA
			//Ez is jó:
			//int status = msgrcv(uzenetsor, &uz, 20, 1, 0);
			
			if (status < 0)
			{
				printf("HIBA  -  Taxis: 1. msgrcv\n");
				exit(1);
			}
			else
			{
				printf( "TAXIS  -  A kapott uzenet szovege: %s\n\n", uz.mtext);
			}
			
			
			
			
			status = msgrcv(uzenetsor, &uz, 1024, 1, 0 );
			
			if (status < 0)
			{
				printf("HIBA  -  Taxis: 2. msgrcv\n");
				exit(1);
			}
			else
			{
				printf( "TAXIS  -  A kapott uzenet szovege: %s\n\n", uz.mtext);
			}
			
			
			
			
			uz.mtype = 2;
			strcpy(uz.mtext, "Elindultam az utasért");
	 
			status = msgsnd(uzenetsor, &uz, sizeof(uz.mtext), 0 );
			
			if (status < 0)
			{
				printf("HIBA  -  Taxis: msgsnd\n");
				exit(1);
			}
			
			
			

			//sendig an integer as an additional data:
			union sigval s_value_int = {getpid()};
			// vagy:
			// union sigval s_value_int;
			// s_value_int.sival_int = getpid();
			sigqueue(utas, SIGUSR1, s_value_int); //just the same as kill function, but we can send additional data too
			
			printf("kortekorte %i  \n", getpid());
			
//*********************** TAXIS VÉGE ***********************
		
		}
		else
		{
			
//*********************** TAXITÁRSASÁG ***********************
			
			close(pipe_FD[1]);
			
			char *lakcim = (char*)malloc(100 * sizeof(char));
			read(pipe_FD[0], lakcim, 100);
			printf("TAXITÁRSASÁG  -  Kiolvasott lakcím: %s\n\n", lakcim);
			
			close(pipe_FD[0]);
			
			
			
			
			struct uzenet uz;
     
			uz.mtype = 1;
			strcpy(uz.mtext, lakcim);
			
			int status_2;
			status_2 = msgsnd(uzenetsor, &uz, 1024, 0 );
			//Ez is jó:
			//status_2 = msgsnd(uzenetsor, &uz, 20, 0 );

			if (status_2 < 0)
			{
				printf("HIBA  -  Taxitársaság: 1. msgsnd\n");
				exit(1);
			}
			
			

			strcpy(uz.mtext, "getpid()");
	 
			status_2;
			status_2 = msgsnd(uzenetsor, &uz, sizeof(uz.mtext), 0 );
			
			if (status_2 < 0)
			{
				printf("HIBA  -  Taxitársaság: 2. msgsnd\n");
				exit(1);
			}
			
			
			
		
			
			
			
			struct uzenet uz_2;
			int status_3;
			
			status_3 = msgrcv(uzenetsor, &uz_2, 1024, 2, 0 );
			
			if ( status_3 < 0 )
			{
				printf("HIBA  -  Taxitársaság: msgrcv\n");
				exit(1);
			}
			else
			{
				printf("TAXITÁRSASÁG  -  A taxistól kapott uzenet szovege: %s\n\n", uz_2.mtext);
			}
			
			
			
			
			int stst;
			waitpid(utas, &stst, 0);
			waitpid(taxis, &stst, 0);
			
			
			
			int status = msgctl( uzenetsor, IPC_RMID, NULL );
			if ( status < 0 ) 
				//perror("msgctl"); 
			return 0;
			
//*********************** TAXITÁRSASÁG VÉGE ***********************
		
		}
	}
	
	return 0;
}