#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
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

int main (int argc,char* argv[]) {

    int pid;
    key_t kulcs;
    int oszt_mem_id;
	
    st *s = malloc (5 * sizeof(st));
	// a parancs nevevel es az 1 verzio szammal kulcs generalas
    kulcs=ftok(argv[0],1);
    // osztott memoria letrehozasa, irasra olvasasra, 500 bajt mrettel
    oszt_mem_id=shmget(kulcs,5 * sizeof(st),IPC_CREAT|S_IRUSR|S_IWUSR);
    // csatlakozunk az osztott memoriahoz, 
    // a 2. parameter akkor kell, ha sajat cimhez akarjuk illeszteni
    // a 3. paraméter lehet SHM_RDONLY, ekkor csak read van   
	s = (st *) shmat(oszt_mem_id,NULL,0);
    //
    pid = fork();
    if(pid>0){    
       
	   int i = 0;
	   for(i ; i< 5; i++)
	   {
		   s[i].i=i;
		   s[i].f[0]='d';
	   }
	   printf("successful writing");
       // elengedjuk az osztott memoriat
       shmdt(s);	   
       wait(NULL); 
	// IPC_RMID- torolni akarjuk a memoriat, ekkor nem kell 3. parameter
	// IPC_STAT- osztott memoria adatlekerdezes a 3. parameterbe,
	//  ami shmid_ds struct tipusu mutato
       shmctl(oszt_mem_id,IPC_RMID,NULL);
    } 
    else 
	if ( pid == 0 ) 
	{
	sleep(1);
		int i = 0;
		for(i; i<5; ++i)
		{
			printf("Az osztott memoriabol olvasta: %d %c\n",s[i].i, s[i].f[0]);
		}
        //printf("A gyerek ezt olvasta az osztott memoriabol: %s",s);
	// gyerek is elengedi
        shmdt(s);
	}

   return 0;
}