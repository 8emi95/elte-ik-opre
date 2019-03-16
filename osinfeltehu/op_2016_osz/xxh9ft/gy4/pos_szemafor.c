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

sem_t* szemafor_letrehozas(char*nev, int szemafor_ertek) {
    sem_t *semid=sem_open(nev,O_CREAT,S_IRUSR|S_IWUSR,szemafor_ertek );
	if (semid==SEM_FAILED){
		perror("sem_open");
	}
       
    return semid;
}

void szemafor_torles(char* nev) {
      sem_unlink(nev);
}

int main (int argc,char* argv[]) {
    pid_t child;
    key_t kulcs;
    int sh_mem_id;
    sem_t* semid;
    char *s;
    char* sem_nev="/xxh9ft"; // Ez egyedi kell legyen, nehogy mas akarja hasznalni

    kulcs = ftok(argv[0], 1);
    sh_mem_id = shmget(kulcs, MEMSIZE, IPC_CREAT|S_IRUSR|S_IWUSR);
    s = shmat(sh_mem_id, NULL, 0);

    semid = szemafor_letrehozas(sem_nev, 0);

    child = fork();
	// parent
    if(child > 0) {    
       	int buffer[5] = {1, 2, 3, 4, 5};

		printf("Szulo indul, kozos memoriaba irja: %d\n", buffer[0]);
       	sleep(1); // wait for a few seconds             
       	s[0] = buffer[0];	// copy data to shared memory
		printf("Szulo, szemafor up!\n");
       	sem_post(semid);	// semaphore up

		   sleep(1);
		printf("Szulo: Varni kezdek a posix szemaforra!\n");
		sem_wait(semid);	// semaphore down
       	s[0] = buffer[1];	// copy data to shared memory
		sem_post(semid);
		printf("Szulo: felengedtem a szemafort!\n");

		// feltakarit maga utan
		shmdt(s);	// release shared memory	
       	wait(NULL);       
       	szemafor_torles(sem_nev);
		shmctl(sh_mem_id,IPC_RMID,NULL);
    } else 
	//child
	if ( child == 0 ) {
		int i=0;
		for(i=0; i<2; ++i){
			/*  kritikus szakasz kezdete   */
			printf("Gyerek: Varni kezdek a posix szemaforra!\n");
			sem_wait(semid);	// semaphore down
			printf("Gyerek, down rendben, eredmeny: %d\n", s[0]);
			sem_post(semid);
			printf("Gyerek: Felengedtem a posix szemafort\n");
			/*  kritikus szakasz vege  */
		   sleep(1);
		}

		// takaritas
		shmdt(s);
	}

   return 0;
}
