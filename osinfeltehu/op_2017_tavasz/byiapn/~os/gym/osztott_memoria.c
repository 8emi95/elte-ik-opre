<sys/shm.h>

shmget(kulcs, m�ret, flagek): megkeres vagy lefoglal egy mem�riater�letet �s bejegyzi, a folyamat l�tni fogja -> shmid vagy -1
	kulcs: ftok(char*, int)
	flagek: hozz�f�r�si enged�lyek �s _IPC_CREAT | _IPC_EXCL
	m�ret: getpagesize() poz. szorzatai. pl: getpagesize()->4096, m�ret: 5 -> 4096. m�ret: 4100 -> 8192
shmat(shmid, NULL, 0): csatlakoztatja a folyamatot -> osztott mem�ria kezd�c�me, shmaddr vagy -1
shmdt(shmaddr): lev�lasztja a folyamatot
shmctl(shmid, IPC_RMID, NULL): felszabad�tja a mem�ri�t �s t�rli a bejegyz�sek k�z�l

p�lda:
osztmem.c