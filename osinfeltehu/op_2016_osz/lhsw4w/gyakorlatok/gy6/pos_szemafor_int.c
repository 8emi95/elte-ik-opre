#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define MEMSIZE 1024

//gcc -pthread pos_semafor

//semafornak mindig kell egy kezdoerteket adni (0--zart, 1--nyitott)
//ha le van zárva akkor a ygerek nem jut hozzá
sem_t* szemafor_letrehozas(char*nev,int szemafor_ertek)
{
    sem_t *semid=sem_open(nev,O_CREAT,S_IRUSR|S_IWUSR,szemafor_ertek );
	if (semid==SEM_FAILED)
	perror("sem_open");
       
    return semid;
}

void szemafor_torles(char* nev)
{
      sem_unlink(nev);
}

int main (int argc,char* argv[]) {

    pid_t child;
    key_t kulcs;
    int sh_mem_id;
    sem_t* semid;
    int *s;
    char* sem_nev="/alma1";
	
	srand(time(NULL)); 
	int r;
	//int r=rand()%50; 

    kulcs=ftok(argv[0],1);
    sh_mem_id=shmget(kulcs,MEMSIZE,IPC_CREAT|S_IRUSR|S_IWUSR);
    s = shmat(sh_mem_id,NULL,0); //osztott memoria amire felcsatlakozunk

    semid = szemafor_letrehozas(sem_nev,0);

    child = fork();
    if(child>0)	// parent
	{    
		int a;
		for(a = 0; a < 5; a++){
			r=rand()%50; 
			int buffer[] = r;		 
		}
       	
	printf("Szulo indul, kozos memoriaba irja: %s\n",buffer);
       	sleep(4); // wait for a few seconds             
       	strcpy(s,buffer);	// copy data to shared memory
	printf("Szulo, szemafor up!\n");
       	sem_post(semid);	// semaphore up (felelngedi a szemafort)
	shmdt(s);	// release shared memory	
       	wait(NULL);       
       	szemafor_torles(sem_nev);
	shmctl(sh_mem_id,IPC_RMID,NULL); //(csak akkor engedi a tenyleges torlest ha mindenki lecsatlakozott rola)
    	} 
	else 
	if ( child == 0 ) //child
		{	
       		/*  kritikus szakasz kezdete   */
		printf("Gyerek: Indul a posix szemafor down!\n");
       		sem_wait(semid);	// semaphore down (lezarja sajat maganak a szemafort)
       		printf("Gyerek, down rendben, eredmeny: %s",s);  
       		sem_post(semid);      
       		/*  kritikus szakasz vége  */  	  
       		shmdt(s);
    		}

   return 0;
}


