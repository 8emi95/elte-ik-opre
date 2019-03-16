#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>


int main (int argc,char* argv[]) {

    int pid;
    key_t kulcs;
    int oszt_mem_id;
    int *s;
	// a parancs nevevel es az 1 verzio szammal kulcs generalas
    kulcs=ftok(argv[0],1);
    // osztott memoria letrehozasa, irasra olvasasra, 500 bajt mrettel
    oszt_mem_id=shmget(kulcs,500,IPC_CREAT|S_IRUSR|S_IWUSR);
    // csatlakozunk az osztott memoriahoz, 
 s = shmat(oszt_mem_id,NULL,0);
    //
    pid = fork();
    if(pid>0){    
    
	s[1]=15;  
        s[0]=5;               
 
       wait(NULL); 
       printf("Szulo: %d\n",s[2]);
       shmdt(s);
       shmctl(oszt_mem_id,IPC_RMID,NULL);
    } 
    else 
	if ( pid == 0 ) 
	{
	sleep(1);
	s[2]=s[0]+s[1];	              
        printf("A gyerek ezt olvasta az osztott memoriabol: %d + %d\n",s[0],s[1]);
	// gyerek is elengedi
        shmdt(s);
	}

   return 0;
}
