#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <string.h>

int szemafor_letrehozas (const char* pathname, int szemafor_ertek){
    int semid;
    key_t kulcs;
    
    kulcs=ftok (pathname, 1);    
    if ((semid=semget (kulcs, 1, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0)
	perror("semget");
    // semget 2. parameter is the number of semaphores   
    if (semctl (semid,0,SETVAL,szemafor_ertek)<0)    //0= first semaphores
        perror ("semctl");
       
    return semid;
}


void szemafor_muvelet (int semid, int op){
    struct sembuf muvelet;
    
    muvelet.sem_num = 0;
    muvelet.sem_op  = op; // op=1 up, op=-1 down 
    muvelet.sem_flg = 0;
    
    if (semop (semid, &muvelet, 1) < 0) // 1 number of sem. operations
        perror ("semop");	    
}

void szemafor_torles (int semid){
      semctl (semid, 0, IPC_RMID);
}


void handler (int signumber) {
  union sigval sv;
	sv.sival_int = rand () % 2;

	sigqueue (getppid (), SIGUSR1, sv);
	printf ("jelzes ertesrol\n");
}


//sigaction handler
static void actionhandler (int sig, siginfo_t *si, void *ucontext)
{
    if (si->si_value.sival_int == 0)
		printf ("nemerti\n");
	else
		printf ("erti");
}


int main (int argc, char* argv[])
{
	int status;
	pid_t child1;
	pid_t child2;
	char buf1[160];
	char buf2[160];
	int pipefd1[2];
	int pipefd2[2];
	int resp1;
	int resp2;
	int rand1;
	int rand2;
	key_t kulcs;
	int sh_mem_id, semid;
	char* s;
		
	kulcs = ftok (argv[0], 1);
	
	sh_mem_id = shmget (kulcs, 1024, IPC_CREAT|S_IRUSR|S_IWUSR);
    s = shmat (sh_mem_id, NULL, 0);
	
	semid = szemafor_letrehozas (argv[0], 0);
	
	if (pipe (pipefd1) == -1)
		perror ("1. pipe");
	
	if (pipe (pipefd2) == -1)
		perror ("2. pipe");
	
	child1 = fork ();
	
	if (child1 == -1)
		perror ("1. Fork");
	
	if (child1 > 0) { //parent
	
		child2 = fork ();
		
		if (child2 == -1)
			perror ("2. Fork");
		
		if (child2 == 0) { //child2
			signal (SIGTERM, handler);
			read (pipefd2[0], buf2, sizeof (buf2));
			printf (buf2);
			srand (time (NULL) ^ (getpid () << 16));
			rand2 = rand () % 5;
			write (pipefd2[1], &rand2, sizeof (rand2));
			szemafor_muvelet (semid, -1);
			rand2 = rand () % 2;
			if (rand2 == 0)
				s[0] = '2';
			szemafor_muvelet (semid, 1);
			shmdt (s);
			pause ();
			
		} else { //parent//
			//sigaction handler
			struct sigaction sa;
			sigemptyset (&sa.sa_mask);
			sa.sa_sigaction = actionhandler;
			sa.sa_flags = SA_SIGINFO; /* Important. */

			sigaction (SIGUSR1, &sa, NULL);
		
			write (pipefd1[1], "kerdes1\n", sizeof ("kerdes1\n"));
			write (pipefd2[1], "kerdes2\n", sizeof ("kerdes2\n"));
			sleep (1);
			read (pipefd2[0], &resp2, sizeof (resp2));
			read (pipefd1[0], &resp1, sizeof (resp1));
			printf ("valasz1: %d\n", resp1);
			printf ("valasz2: %d\n", resp2);
			szemafor_muvelet (semid, 1);
			sleep (1);
			szemafor_muvelet (semid, -1);
			printf ("2. erti-e? : %c\n", s[0]);
			printf ("1. erti-e? : %c\n", s[1]);
			sleep (1);
			kill (child1, SIGTERM);
			sleep (1);
			kill (child2, SIGTERM);
			pause ();
			waitpid (child1, &status, 0);
			waitpid (child2, &status, 0);
			szemafor_torles (semid);
			shmdt (s);
			shmctl (sh_mem_id, IPC_RMID, NULL);
		}
	} else { //child1
		signal (SIGTERM, handler);
		read (pipefd1[0], buf1, sizeof (buf1));
		printf (buf1);
		srand (time (NULL) ^ (getpid () << 16));
		rand1 = rand () % 5;
		write (pipefd1[1], &rand1, sizeof (rand1));
		szemafor_muvelet (semid, -1);
		rand1 = rand () % 2;
		if (rand1 == 0)
			s[1] = '1';
		szemafor_muvelet (semid, 1);
		shmdt (s);
		pause ();
	}
	
	return 0;
}
