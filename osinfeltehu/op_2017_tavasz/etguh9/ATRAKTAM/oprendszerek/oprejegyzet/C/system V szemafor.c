SZEMAFOR
(nem posX-es)



-> LÉTREHOZÁS:

	int semid;
	key_t kulcs;
	kulcs = ftok(__pathname__, 1);

	if ((semid = semget(kulcs, 1, IPC_CREAT|S_IRUSR|S_IWUSR )) < 0)
		printf("HIBA\n");
   


-> BEÁLLÍTÁS (LÉTREHOZÁS UTÁN, a, család minden tagjára meg kell csinálni):

Adott:
- int semid  -  szemafor azonosító
- int(??) semafor_Value  -  kezdőérték (le(-1) / fel(1))

	if (semctl(semid, 0, SETVAL, szemafor_ertek) < 0)
		//0 = first semaphores
		printf("HIBA\n");



-> ÁLLÍTÁS (MENET KÖZBEN):

Adott:
- int semid  -  szemafor azonosító
- int op  -  le(-1) / fel(1)

	struct sembuf muvelet;
		
	muvelet.sem_num = 0;
	muvelet.sem_op  = op;
	muvelet.sem_flg = 0;   -   "akkor használatos, ha ugy akarom használni a szemafor, hogy ha záérva van, akkor nem akarok várni, hanem továbba akarok menni (vagyis ránlézek a szemaforra és ha azt látom hogy zárt, akkor továbbmergyek.) ilyenjkor ez a flag nem nulla (de mi??)"
		
	if (semop(semid, &muvelet, 1) < 0)
		//1 number of sem. operations
		printf("HIBA\n");



-> TÖRLÉS:

Adott:
- int semid  -  szemafor azonosító

	semctl(semid, 0, IPC_RMID);