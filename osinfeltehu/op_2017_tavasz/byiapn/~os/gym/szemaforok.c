include <sys/sem.h>

semget(kulcs, szemaforok sz�ma (x), flagek): l�trehoz x db szemafort, inicializ�lni kell. -> semid, vagy -1
semop(semid, &m�veletek t�mbje, m�veletek sz�ma): v�grehajtja a m�veleteket a szemaforokon. 
	m�veletek t�mbje: sembuf (struct) { sem_num (unsigned int), sem_flag (short): _IPC_NOWAIT _SEM_UNDO _O, sem_op: m�velet }
				sem_op: x>0 - hozz�ad x-et 
					x=0 - v�r a null�rt�kre
					x<0 - v�r a cs�kkent�sre, cs�kkent ha tud
semctl(semid, szemafor, parancs[, �rt�k]):
	parancs: SETVAL : inicializ�lja '�rt�k'-re
		 IPC_RMID: t�rli a szemafor csal�dot

POSIX-os szemaforcsal�d:
ford�t�shoz kell -lpthread

include <semaphore.h>

sem_init: n�v n�lk�li szemafor
sem_open(n�v, m�d[, enged�lyek, kezd��rt�k]): l�trehoz vagy csatlakozik egy szemaforhoz -> semaddr
sem_post(semaddr): 1-gyel n�vel -> 0 siker -1 ha nem
sem_wait(semaddr): 1-gyel cs�kkent, ha tud
sem_trywait(semaddr): pr�b�l cs�kkenteni, nem v�r
sem_timewait(semaddr, timespec): v�r az adott id�pontra
sem_unlink(n�v)
