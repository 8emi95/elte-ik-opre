include <sys/sem.h>

semget(kulcs, szemaforok száma (x), flagek): létrehoz x db szemafort, inicializálni kell. -> semid, vagy -1
semop(semid, &mûveletek tömbje, mûveletek száma): végrehajtja a mûveleteket a szemaforokon. 
	mûveletek tömbje: sembuf (struct) { sem_num (unsigned int), sem_flag (short): _IPC_NOWAIT _SEM_UNDO _O, sem_op: mûvelet }
				sem_op: x>0 - hozzáad x-et 
					x=0 - vár a nullértékre
					x<0 - vár a csökkentésre, csökkent ha tud
semctl(semid, szemafor, parancs[, érték]):
	parancs: SETVAL : inicializálja 'érték'-re
		 IPC_RMID: törli a szemafor családot

POSIX-os szemaforcsalád:
fordításhoz kell -lpthread

include <semaphore.h>

sem_init: név nélküli szemafor
sem_open(név, mód[, engedélyek, kezdõérték]): létrehoz vagy csatlakozik egy szemaforhoz -> semaddr
sem_post(semaddr): 1-gyel növel -> 0 siker -1 ha nem
sem_wait(semaddr): 1-gyel csökkent, ha tud
sem_trywait(semaddr): próbál csökkenteni, nem vár
sem_timewait(semaddr, timespec): vár az adott idõpontra
sem_unlink(név)
