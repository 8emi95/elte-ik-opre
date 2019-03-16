SZEMAFOR
(posX-es)

Fordítás:
gcc -lrt valami.c
"-pthread kapcsolóval kell fordítani"



-> LÉTREHOZÁS:

Adott:
- char* nev  -  leendõ szemafor neve (pl:.: "/alma1", de érdemes a saját neptunkódot használni)
- int szemafor_ertek  -  kezdõérték //0 volt a példakódban

	sem_t *semid = sem_open(nev, O_CREAT, S_IRUSR|S_IWUSR, szemafor_ertek);
	if (semid == SEM_FAILED)
		printf("HIBA\n");



-> ÁLLÍTÁS (MENET KÖZBEN):

Adott:
- semid  -  szemafor azonosító

	sem_post(semid);   //semaphore up
	sem_wait(semid);   //semaphore down



-> TÖRLÉS:

Adott:
- char* nev  -  szemafor név

	sem_unlink(nev);