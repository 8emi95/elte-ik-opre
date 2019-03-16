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
    int szamok[5] = {1,2,3,4,5};
    char *s; // ez atirthato barmilyen tipusra
//int * s;
	// a parancs nevevel es az 1 verzio szammal kulcs generalas
    kulcs=ftok(argv[0],1);
    // osztott memoria letrehozasa, irasra olvasasra, 500 bajt merettel
    oszt_mem_id=shmget(kulcs,500,IPC_CREAT|S_IRUSR|S_IWUSR);
    // csatlakozunk az osztott memoriahoz, 
    // a 2. parameter akkor kell, ha sajat cimhez akarjuk illeszteni
    // a 3. paraméter lehet SHM_RDONLY, ekkor csak read van   
 s = shmat(oszt_mem_id,NULL,0); // void* -ot ad vissza, ezert pl. ha inteket akarok belerakni, akkor nem kell konvertalgatni
    //
    pid = fork();
    if(pid>0){    
    // 1.
	//sleep(1);
//	srand(time(NULL));    
   char buffer[] = "Hajra Fradi! \n";                   
       // beirunk a memoriaba 
       strcpy(s,buffer);
       // elengedjuk az osztott memoriat
       shmdt(s);	   
//	s[0]='B';  //ez segmentation fault hibat ad
       wait(NULL); 
	// IPC_RMID- torolni akarjuk a memoriat, ekkor nem kell 3. parameter
	// IPC STAT- osztott memoria adatlekerdezes a 3. parameterbe,
	//  ami shmid ds struct tipusu mutato
// 2. kiszedem a torlest    
	shmctl(oszt_mem_id,IPC_RMID,NULL);
    } 
    else 
	if ( pid == 0 ) 
	{
		// 1. ezt kitakarjuk es atrakjuk az elozo if be
	sleep(1);	              
/*	int i;
	for(i=0;i<?;i++)
{
	printf(?);
}*/
        printf("A gyerek ezt olvasta az osztott memoriabol: %s",s);
	// gyerek is elengedi
        shmdt(s);
	}

   return 0;
}
