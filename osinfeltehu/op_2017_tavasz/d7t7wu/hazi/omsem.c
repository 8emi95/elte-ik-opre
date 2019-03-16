#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

typedef struct
{
	int i;
	char f[1];
}st;

int szemafor_letrehozas(const char* pathname,int szemafor_ertek){
    int semid;
    key_t kulcs;
    
    kulcs=ftok(pathname,1);    
    if((semid=semget(kulcs,1,IPC_CREAT|S_IRUSR|S_IWUSR ))<0)
	perror("semget");
    // semget 2. parameter is the number of semaphores   
    if(semctl(semid,0,SETVAL,szemafor_ertek)<0)    //0= first semaphores
        perror("semctl");
    //^e nelkul nem szabad elinditani   
    return semid;
}


void szemafor_muvelet(int semid, int op){
    struct sembuf muvelet;
    
    muvelet.sem_num = 0;
    muvelet.sem_op  = op; // op=1 up, op=-1 down 
    muvelet.sem_flg = 0;
    
    if(semop(semid,&muvelet,1)<0) // 1 number of sem. operations
        perror("semop");	    
}

void szemafor_torles(int semid){
      semctl(semid,0,IPC_RMID);
}

int main (int argc,char* argv[]) {

    int pid;
    key_t kulcs;
    int oszt_mem_id;
	int semid;
	
    st *s = malloc (5 * sizeof(st));
	// a parancs nevevel es az 1 verzio szammal kulcs generalas
    kulcs=ftok(argv[0],1);
    oszt_mem_id=shmget(kulcs,5 * sizeof(st),IPC_CREAT|S_IRUSR|S_IWUSR);
	s = (st *) shmat(oszt_mem_id,NULL,0);
	
	semid = szemafor_letrehozas(argv[0], 0);
    //
    pid = fork();
    if(pid>0){    
       sleep(3);
	   int i = 0;
	   for(i ; i< 5; i++)
	   {
		   s[i].i=i;
		   s[i].f[0]='d';
	   }
	   //sem_post(semid);
	   printf("successful writing, sem up\n");
	   szemafor_muvelet(semid,1); // Up
       // elengedjuk az osztott memoriat
       shmdt(s);	   
       wait(NULL);  
       szemafor_torles(semid);
	// IPC_RMID- torolni akarjuk a memoriat, ekkor nem kell 3. parameter
	// IPC_STAT- osztott memoria adatlekerdezes a 3. parameterbe,
	//  ami shmid_ds struct tipusu mutato
       shmctl(oszt_mem_id,IPC_RMID,NULL);
    } 
    else 
	if ( pid == 0 ) 
	{
	sleep(1);
	
		// critical section
		printf("Gyerek: Indula szemafor down!\n");
		szemafor_muvelet(semid,-1); // down, wait if necessary
		int i = 0;
		
		for(i; i<5; ++i)
		{
			printf("Az osztott memoriabol olvasta: %d %c\n",s[i].i, s[i].f[0]);
		}
		szemafor_muvelet(semid,1); // up     

        printf("A gyerek ezt olvasta az osztott memoriabol, miutan felemlodott a szemafor\n");
	// gyerek is elengedi
        shmdt(s);
	}

   return 0;
}
