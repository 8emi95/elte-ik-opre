SZEMAFOR
(posX-es)

Ford�t�s:
gcc -lrt valami.c
"-pthread kapcsol�val kell ford�tani"



-> L�TREHOZ�S:

Adott:
- char* nev  -  leend� szemafor neve (pl:.: "/alma1", de �rdemes a saj�t neptunk�dot haszn�lni)
- int szemafor_ertek  -  kezd��rt�k //0 volt a p�ldak�dban

	sem_t *semid = sem_open(nev, O_CREAT, S_IRUSR|S_IWUSR, szemafor_ertek);
	if (semid == SEM_FAILED)
		printf("HIBA\n");



-> �LL�T�S (MENET K�ZBEN):

Adott:
- semid  -  szemafor azonos�t�

	sem_post(semid);   //semaphore up
	sem_wait(semid);   //semaphore down



-> T�RL�S:

Adott:
- char* nev  -  szemafor n�v

	sem_unlink(nev);