#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>


//FELADAT: szulo kuld 2 szamot, gyerek osszeadja, szulo kiirja

int main (int argc,char* argv[]) {

    int pid;
    key_t kulcs;
    int oszt_mem_id;

    int *s;
    //char *s
    //int *i = s + 100
    //igy  az elso 100 char lesz, utana a maradek 400 bajt int (100db int)
    
	   // a parancs nevevel es az 1 verzio szammal kulcs generalas
     //ftok - egyedi azonosito parameterrel IPC_PRIVATE (shmget(IPC_PRIVATE, ...) nem kell ez a kulcs/ftok hivas)
    kulcs = ftok(argv[0], 1);
    // osztott memoria letrehozasa, irasra olvasasra, 500 bajt mrettel
    oszt_mem_id = shmget(kulcs, 500, IPC_CREAT|S_IRUSR|S_IWUSR);
    // csatlakozunk az osztott memoriahoz,
    // a 2. parameter akkor kell, ha sajat cimhez akarjuk illeszteni - memoriaterulet van itt megadva (akar elore definialt tomb is lehet)
    // a 3. paraméter lehet SHM_RDONLY, ekkor csak read van
    s = shmat(oszt_mem_id, NULL, 0);

    pid = fork();
    if(pid > 0){

       s[0] = 2;
       s[1] = 10;

       wait(NULL); //a gyerekfolyamat veget varja meg

    	// IPC_RMID- torolni akarjuk a memoriat, ekkor nem kell 3. parameter
    	// IPC_STAT- osztott memoria adatlekerdezes a 3. parameterbe,
    	//  ami shmid_ds struct tipusu mutato
      printf("A szulo olvasta: %d\n", s[2]);

      shmdt(s);
      shmctl(oszt_mem_id, IPC_RMID, NULL);
    }
    else
    	if ( pid == 0 )
    	{
    	      sleep(1);
            printf("A gyerek ezt olvasta az osztott memoriabol: %d, %d\n",s[0], s[1]);
            s[2] = s[0] + s[1];
    	       // gyerek is elengedi
            shmdt(s);
    	}

   return 0;
}
