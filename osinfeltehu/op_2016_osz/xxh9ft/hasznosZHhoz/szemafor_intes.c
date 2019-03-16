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

int szemafor_letrehozas(const char* pathname, int szemafor_ertek) {
    int semid;
    key_t kulcs;
    
    kulcs = ftok(pathname, 1);
    if((semid=semget(kulcs, 1, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
    	perror("semget");
    }
    // semget 2. parameter is the number of semaphores
    if(semctl(semid,0,SETVAL,szemafor_ertek)<0) {    //0= first semaphores
        perror("semctl");
    }

    return semid;
}

void szemafor_muvelet(int semid, int op) {
    struct sembuf muvelet;
    
    muvelet.sem_num = 0;
    muvelet.sem_op  = op; // op=1 up, op=-1 down 
    muvelet.sem_flg = 0;
    
    // 1 number of sem. operations
    if(semop(semid,&muvelet,1)<0) {
        perror("semop");	    
    }
}

void szemafor_torles(int semid) {
      semctl(semid,0,IPC_RMID);
}

int main (int argc,char* argv[]) {
    pid_t child;
    key_t kulcs;
    int sh_mem_id,semid;
    int *s;

    kulcs=ftok(argv[0],1);
    sh_mem_id=shmget(kulcs,MEMSIZE,IPC_CREAT|S_IRUSR|S_IWUSR);
    s = shmat(sh_mem_id, NULL, 0);
    //
    semid = szemafor_letrehozas(argv[0], 0); // sem state is down!!! lezarva hozzuk letre
    //
    child = fork();
    if(child>0) {
        int buffer[] = {1,2,3};
        printf("Szulo indul, kozos memoriaba irja: %d %d %d\n", buffer[0], buffer[1], buffer[2]);
        sleep(4);            // child waits during sleep

        int i=0;
        for(i=0; i<sizeof(buffer); ++i){
            s[i] = buffer[i];
        }

        printf("Szulo, szemafor up!\n");
        szemafor_muvelet(semid, 1); // Up

        // takaritas
        shmdt(s);	// release shared memory
        wait(NULL);
        szemafor_torles(semid);
        shmctl(sh_mem_id, IPC_RMID, NULL);

    } else if ( child == 0 ) {
        printf("Gyerek: Indula szemafor down!\n");
        // critical section
        szemafor_muvelet(semid, -1); // down, wait if necessary
        printf("Gyerek, down rendben, eredmeny: %d %d %d\n", s[0], s[1], s[2]);
        szemafor_muvelet(semid, 1); // up
        // end of critical sectionS

        shmdt(s);
    }

   return 0;
}
