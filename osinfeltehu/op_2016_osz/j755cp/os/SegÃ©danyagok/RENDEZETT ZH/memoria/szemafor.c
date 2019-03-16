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
	//semafor inicializ�l�sa
	key_t key = ftok(argv[0], 1); //kulcs szemaforhoz (osztott memhez is j�)
	int semid = semget(key, 1/*h�ny szemafor van*/,IPC_CREAT | 0600); //szemafor id be�ll�t�sa
	union semun base;
	base.val = 1; //nyitott �llapotban j�n l�tre a szemafor (0 - z�rt)
	semctl(semid, 0/*h�nyadik szemaforra vonatkozik a m�velet*/, SETVAL/*�rt�k be�ll�t�sa*/, &base/*erre*/);
	
	//semafor le�ll�t�sa
	struct sembuf sb;
	sb.sem_num = 0; //h�nyadik szemafor 
	sb.sem_op = -1; //ez mindig ez a le�ll�shoz
	semop(semid, &sb, 1); //szemafor le�ll�t�sa
	
	//szemafor megnyit�sa //kritikus m�velet
	sb.sem_op = 1;
	semop(semid, &sb, 1);
	semctl(semid, IPC_RMID, 0);
	
	return 0;
}