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
    int* s;
	// a parancs nevevel es az 1 verzio szammal kulcs generalas
    kulcs=ftok(argv[0],1);
    // osztott memoria letrehozasa, irasra olvasasra, 500 bajt mrettel
    oszt_mem_id=shmget(kulcs,500,IPC_CREAT|S_IRUSR|S_IWUSR);
    // csatlakozunk az osztott memoriahoz,
    // a 2. parameter akkor kell, ha sajat cimhez akarjuk illeszteni
    // a 3. paraméter lehet SHM_RDONLY, ekkor csak read van   
	s = shmat(oszt_mem_id,NULL,0);
    //
    pid = fork();
    if(pid>0){    
		
       s[0] = 5;
	   s[1] = 13;	
       // beirunk a memoriaba 
       // strcpy(sz,sz);
       // elengedjuk az osztott memoriat
       // shmdt(s);	   
//	s[0]='B';  //ez segmentation fault hibat ad
       wait(NULL);
	// IPC_RMID- torolni akarjuk a memoriat, ekkor nem kell 3. parameter
	// IPC_STAT- osztott memoria adatlekerdezes a 3. parameterbe,
	//  ami shmid_ds struct tipusu mutato
	printf("A szamok osszege: %d \n",s[2]);
	shmdt(s);
	
       shmctl(oszt_mem_id,IPC_RMID,NULL);
    } 
    else 
	if ( pid == 0 ) 
	{
		sleep(1);	              
        printf("A gyerek ezt olvasta az osztott memoriabol: %d, %d\n",s[0],s[1]);
		s[2]=s[0]+s[1];
		shmdt(s);
		
	}

   return 0;
}
