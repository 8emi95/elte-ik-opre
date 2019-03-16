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
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>




struct uzenet
{
    long mtype;
    char mtext[1024];
};




void sighandler() {};




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
	
	
	
	
	int semid;

	if ((semid = semget(kulcs, 1, IPC_CREAT|S_IRUSR|S_IWUSR )) < 0)
		printf("HIBA\n");
	
	if (semctl(semid, 0, SETVAL, 1) < 0)
	{
		printf("HIBA  -  semctl\n");
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
	
	
	
	
	pid_t ugynok, vasarlo;
	
	ugynok = fork();

	if (ugynok == -1)
	{
		printf("HIBA  -  1. fork\n");
		exit(1);
	}
	if (ugynok == 0)
	{
		
//*********************** ÜGYNÖK ***********************
		
		struct uzenet uz;
		int status;
		
		status = msgrcv(uzenetsor, &uz, 1024, 1, 0 );
		
		if ( status < 0 )
		{
			printf("HIBA  -  Ugynok: msgrcv\n");
			exit(1);
		}
		
		pid_t vasarlo_Pid = atoi(uz.mtext);
		
		printf("UGYNOK  -  A vasarlo PID-je: %i\n\n", vasarlo_Pid);
		
		kill(vasarlo_Pid, SIGUSR1);
		
		
		
		
		int eredetiar = atoi(argv[2]);
		int ar = eredetiar + 1;
		
		char c = 'n';
		
		while (c == 'n')
		{
			write(pipe_FD[1], argv[1], 1 + strlen(argv[1]));
		
			ar = ar - 1;
			if (ar < eredetiar / 2)				
			{
				ar = eredetiar;
			}
		
			char *arstr = (char*)malloc(100 * sizeof(char));
			sprintf(arstr, "%d", ar);
		
			write(pipe_FD[1], arstr, 1 + strlen(arstr));
			
			free(arstr);
			
			sigfillset(&sig_Set);
			sigdelset(&sig_Set, SIGUSR1);
			sigsuspend(&sig_Set);
			
			char *s = (char*)malloc(1);
			read(pipe_FD[0], s, 2);
			printf("UGYNOK  -  Vasarlo valasza: %s\n\n", s);
			c = s[0];
			free(s);
			
			kill(vasarlo_Pid, SIGUSR1);
		}
		
		close(pipe_FD[1]);
		close(pipe_FD[0]);
		
		
		
		
		char *ssss;
		ssss = shmat(sh_mem_id, NULL, 0);
		
		struct sembuf muvelet;
		
		muvelet.sem_num = 0;
		muvelet.sem_op  = -1;
		muvelet.sem_flg = 0;
			
		if (semop(semid, &muvelet, 1) < 0)		
		{
			printf("HIBA  -  Ugynok: semop\n");
			exit(1);
		}
			
			
		char *arstr = (char*)malloc(100 * sizeof(char));
		sprintf(ssss, "%d", ar);
		
		
		muvelet.sem_num = 0;
		muvelet.sem_op  = 1;
		muvelet.sem_flg = 0;
			
		if (semop(semid, &muvelet, 1) < 0)
		{
			printf("HIBA  -  Ugynok: semop\n");
			exit(1);
		}
			
		
		kill(getppid(), SIGUSR1);
		
		shmdt(ssss);
		
//*********************** ÜGYNÖK VÉGE ***********************
	
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
			
//*********************** VÁSÁRLÓ ***********************

			sigfillset(&sig_Set);
			sigdelset(&sig_Set, SIGUSR1);
			sigsuspend(&sig_Set);
				
			printf("VASARLO  -  Fogadtam a hivast\n\n");
			
			
			
			
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
				free(s);
				
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
			
			close(pipe_FD[0]);
			close(pipe_FD[1]);
			
//*********************** VÁSÁRLÓ VÉGE ***********************
		
		}
		else
		{
			
//*********************** VÁLLALAT ***********************
			
			close(pipe_FD[0]);
			close(pipe_FD[1]);
			
			char *sss = (char*)malloc(100 * sizeof(char));
			sprintf(sss, "%d", vasarlo);
			
			struct uzenet uz;
			
			uz.mtype = 1;
			strcpy(uz.mtext, sss);
			free(sss);
			
			int status;
			status = msgsnd(uzenetsor, &uz, 1024, 0 );
			
			if (status < 0)
			{
				printf("HIBA  -  Vallalat: msgsnd\n");
				exit(1);
			}
			
			
			
			
			// struct sembuf muvelet;
			
			// muvelet.sem_num = 0;
			// muvelet.sem_op  = -1;
			// muvelet.sem_flg = 0;
				
			// if (semop(semid, &muvelet, 1) < 0)		
			// {
				// printf("HIBA  -  Vallalat: semop\n");
				// exit(1);
			// }
		
		
			char *ssss;
			ssss = shmat(sh_mem_id, NULL, 0);
			
			
			// muvelet.sem_num = 0;
			// muvelet.sem_op  = 1;
			// muvelet.sem_flg = 0;
				
			// if (semop(semid, &muvelet, 1) < 0)		
			// {
				// printf("HIBA  -  Vallalat: semop\n");
				// exit(1);
			// }
			
			
			
			
			sigfillset(&sig_Set);
			sigdelset(&sig_Set, SIGUSR1);
			sigsuspend(&sig_Set);
			
			printf("VALLALAT  -  Vegleges ar: %s\n\n", ssss);
				
			shmdt(ssss);
			
			
			
			
			wait(NULL);
			wait(NULL);
			
			shmctl(sh_mem_id, IPC_RMID, NULL);
			
			semctl(semid, 0, IPC_RMID);
			
			status = msgctl( uzenetsor, IPC_RMID, NULL );
			if ( status < 0 )
			{
				printf("HIBA  -  Vallalat: msgctl\n");
				exit(1);
			}
			
			return 0;
			
//*********************** VÁLLALAT VÉGE ***********************
		
		}
	}
	
	return 0;
}