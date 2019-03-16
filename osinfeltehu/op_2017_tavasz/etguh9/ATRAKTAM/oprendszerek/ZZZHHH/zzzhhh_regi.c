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

#include <semaphore.h>



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
	if (argc != 3)
	{
		printf("Hiba! A program pontosan ketto argumentummal hívható!\n");
		exit(1);
	}
	
	printf("\nVALLALAT\n\n\n");
	
	srand(time(NULL));
	
			
			
			
	key_t kulcs = ftok(argv[0],1);

	int uzenetsor = msgget( kulcs, 0600 | IPC_CREAT );
	if (uzenetsor < 0)
	{ 
		printf("HIBA  -  msgget\n");
		exit(1);
	}
	
	
	
	
	int sh_mem_id = shmget(kulcs, 500, IPC_CREAT|S_IRUSR|S_IWUSR);
	
	
	
	
	sem_t *semid = sem_open("/etguh9", O_CREAT, S_IRUSR|S_IWUSR, 0);
	if (semid == SEM_FAILED)
		printf("HIBA\n");
	
	
	
	
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
	
	
	
	
	pid_t ugynok, vasarlo;
	
	ugynok = fork();

	if (ugynok == -1)
	{
		printf("HIBA  -  1. fork\n");
		exit(1);
	}
	if (ugynok == 0)
	{
		
//*********************** ugynok ***********************
		
		struct uzenet uz;
		int status;
		
		status = msgrcv(uzenetsor, &uz, 1024, 1, 0 );
		
		if ( status < 0 )
		{
			printf("HIBA  -  Ugynok: msgrcv\n");
			exit(1);
		}
		
		pid_t vasarlo_Pid = atoi(uz.mtext);
		
		printf("UGYNOK  -  A vasarlo PId-je: %i\n", vasarlo_Pid);
		
		kill(vasarlo_Pid, SIGUSR1);
		
		
		
		
		int eredetiar = atoi(argv[2]);
		int ar = eredetiar;
		
		char c = 'n';
		
		while (c == 'n')
		{
			write(pipe_FD[1], argv[1], 1 + strlen(argv[1]));
		
			char *arstr = (char*)malloc(100 * sizeof(char));
			sprintf(arstr, "%d", ar);
		
			write(pipe_FD[1], arstr, 1 + strlen(arstr));
			
			ar = ar - 1;
			if (ar < eredetiar / 2)
				ar = eredetiar;
			
			sigfillset(&sig_Set);
			sigdelset(&sig_Set, SIGUSR1);
			sigsuspend(&sig_Set);
			
			char *s = (char*)malloc(1);
			read(pipe_FD[0], s, 2);
			printf("UGYNOK  -  Vasarlo valasza: %s\n", s);
			c = s[0];
			free(s);
			
			kill(vasarlo_Pid, SIGUSR1);
		}
		
		close(pipe_FD[1]);
		close(pipe_FD[0]);
		
		
		
		
		char *ssss;
		ssss = shmat(sh_mem_id, NULL, 0);
		
		shmdt(ssss);
		
//*********************** ugynok VÉGE ***********************
	
	}
	else 
	{		
		vasarlo = fork();
		
		if (vasarlo == -1)
		{
			printf("HIBA  -  2. fork\n");
			exit(1);
		}
		if (vasarlo == 0)
		{
			
//*********************** vasarlo ***********************

			sigfillset(&sig_Set);
			sigdelset(&sig_Set, SIGUSR1);
			sigsuspend(&sig_Set);
				
			printf("VASARLO  -  Fogadtam a hivast\n");
			
			
			
			
			int r = 0;
			
			while (r == 0)
			{
				char *s = (char*)malloc((1 + strlen(argv[1])) * sizeof(char));
				read(pipe_FD[0], s, 1 + strlen(argv[1]));
				printf("VASARLO  -  Kiolvasott termek: %s\n", s);
				free(s);
				s = (char*)malloc((1 + strlen(argv[1])) * sizeof(char));
				read(pipe_FD[0], s, 1 + strlen(argv[2]));
				printf("VASARLO  -  Kiolvasott ar: %s\n\n", s);		
				
				r = rand() % 2;
				
				if (r == 0)
				{
					char *s = "n";
					write(pipe_FD[1], s, 1 + strlen(s));
				}
				else
				{
					char *s = "i";
					write(pipe_FD[1], s, 1 + strlen(s));
				}
				
				kill(ugynok, SIGUSR1);
				
				sigfillset(&sig_Set);
				sigdelset(&sig_Set, SIGUSR1);
				sigsuspend(&sig_Set);
			}
			
			close(pipe_FD[1]);
			close(pipe_FD[0]);
			
//*********************** vasarlo VÉGE ***********************
		
		}
		else
		{
			
//*********************** TAXITÁRSASÁG ***********************
			
			char *valami = (char*)malloc(100 * sizeof(char));
			sprintf(valami, "%d", vasarlo);
			
			//write(pipe_FD[1], valami, 1 + strlen(valami));
			//close(pipe_FD[0]);
			//close(pipe_FD[1]);
			
			struct uzenet uz;
			
			uz.mtype = 1;
			strcpy(uz.mtext, valami);
			
			int status;
			status = msgsnd(uzenetsor, &uz, 1024, 0 );
			// //Ez is jó:
			// //status_2 = msgsnd(uzenetsor, &uz, 20, 0 );

			if (status < 0)
			{
				printf("HIBA  -  Taxitársaság: 1. msgsnd\n");
				exit(1);
			}
			
			
			
			
			char *ssss;
			ssss = shmat(sh_mem_id, NULL, 0);
			
			shmdt(ssss);
			
			shmctl(sh_mem_id, IPC_RMID, NULL);
			
			
			
			
			int stst;
			waitpid(ugynok, &stst, 0);
			waitpid(vasarlo, &stst, 0);
			
			semctl(semid, 0, IPC_RMID);
			
			// int status = msgctl( uzenetsor, IPC_RMID, NULL );
			// if ( status < 0 ) 
				// //perror("msgctl"); 
			// return 0;
			
//*********************** TAXITÁRSASÁG VÉGE ***********************
		
		}
	}
	
	return 0;
}