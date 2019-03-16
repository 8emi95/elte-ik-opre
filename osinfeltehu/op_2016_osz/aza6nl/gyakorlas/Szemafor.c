#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define MEMSIZE 1024

int szemafor_letrehozas(const char* pathname,int szemafor_ertek)
{
    int semid;
    key_t kulcs;
    
    kulcs=ftok(pathname,1);    
    if((semid=semget(kulcs,1,IPC_CREAT|S_IRUSR|S_IWUSR ))<0) perror("semget");

    // semget 2. parameter is the number of semaphores  
	//0= first semaphores
    if(semctl(semid,0,SETVAL,szemafor_ertek)<0) perror("semctl");
    
    return semid;
}

void szemafor_muvelet(int semid, int op)
{
    struct sembuf muvelet;
    
    muvelet.sem_num = 0;
	// op=1 up, op=-1 down 
    muvelet.sem_op  = op; 
    muvelet.sem_flg = 0;
    
	// 1 number of sem. operations
    if(semop(semid,&muvelet,1)<0) perror("semop");	    
}

void szemafor_torles(int semid)
{
	semctl(semid,0,IPC_RMID);
}

int main (int argc,char* argv[])
{
	pid_t child;
	key_t kulcs;
	int sh_mem_id,semid;
	char *s;

	kulcs=ftok(argv[0],1);
	sh_mem_id=shmget(kulcs,MEMSIZE,IPC_CREAT|S_IRUSR|S_IWUSR);
	s = shmat(sh_mem_id,NULL,0);
	
	// sem state is down!!!
	semid = szemafor_letrehozas(argv[0],0); 
	
	child = fork();
	if(child>0)
	{ 
			printf("Szulo indul!\n");
			printf("Szulo szemafor le!\n");
			char buffer[] = "aaaaaaaaaaaaaaaaaaa\n";
			sleep(4);
			printf("Szulo a kozos memoriaba irja: %s\n",buffer);
			strcpy(s,buffer);
			// elengedjuk az osztott memoriat
			shmdt(s);
		szemafor_muvelet(semid,1);
		printf("Szulo, szemafor fel!\n");
		wait( NULL );       
		szemafor_torles(semid);
		shmctl(sh_mem_id,IPC_RMID,NULL);
		printf("Szulo leall!\n");
	}
	else
	{
		printf("Gyerek indul!\n");
		szemafor_muvelet(semid,-1);
			printf("Gyerek szemafor le!\n");
			printf("Gyerek kozos memoriabol olvas: %s",s); 
			// elengedjuk az osztott memoriat
			shmdt(s);
		szemafor_muvelet(semid,1);
		printf("Gyerek szemafor fel!\n");
		printf("Gyerek leall!\n");
	}
	return 0;
}


