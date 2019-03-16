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
    kulcs=ftok(argv[0],1);
    oszt_mem_id=shmget(kulcs,500,IPC_CREAT|S_IRUSR|S_IWUSR);
 s = shmat(oszt_mem_id,NULL,0);
    //
    pid = fork();
    if(pid>0){    
    
       int buffer[] = {1,2,3,4,5};                   
       // beirunk a memoriaba 
       int i;
       for(i=1;i<4;++i)
       {
           s[i]=buffer[i];
       }
        for(i=1;i<4;++i)
        {
            printf("%i\n",s[i]);
        }
       // elengedjuk az osztott memoriat
       shmdt(s);	   
//	s[0]='B';  //ez segmentation fault hibat ad
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
    int i;
    int tart[3];
        for(i=0;i<3;++i)
        {
           tart[i]=s[i];
        }
        for(i=0;i<3;++i)
        {
            printf("%i\n",tart[i]);
        }
	// gyerek is elengedi
        shmdt(s);
	}

   return 0;
}
