#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
int val; /* value for SETVAL */
struct semid_ds *buf; /* buffer for IPC_STAT, IPC_SET */
unsigned short *array; /* array for GETALL, SETALL */
struct seminfo *__buf; /* buffer for IPC_INFO */ 
};


int main(int argc, char** argv)
{
	//semafor inicializálása
	key_t key = ftok(argv[0], 1); //kulcs szemaforhoz (osztott memhez is jó)
	int semid = semget(key, 1/*hány szemafor van*/,IPC_CREAT | 0600); //szemafor id beállítása
	union semun base;
	base.val = 1; //nyitott állapotban jön létre a szemafor (0 - zárt)
	semctl(semid, 0/*hányadik szemaforra vonatkozik a mûvelet*/, SETVAL/*érték beállítása*/, &base/*erre*/);
	
	//semafor leállítása
	struct sembuf sb;
	sb.sem_num = 0; //hányadik szemafor 
	sb.sem_op = -1; //ez mindig ez a leálláshoz
	semop(semid, &sb, 1); //szemafor leállítása
	
	//szemafor megnyitása //kritikus mûvelet
	sb.sem_op = 1;
	semop(semid, &sb, 1);
	semctl(semid, IPC_RMID, 0);
	
	return 0;
}