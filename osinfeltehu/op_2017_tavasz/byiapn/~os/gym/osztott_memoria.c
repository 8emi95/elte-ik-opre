<sys/shm.h>

shmget(kulcs, méret, flagek): megkeres vagy lefoglal egy memóriaterületet és bejegyzi, a folyamat látni fogja -> shmid vagy -1
	kulcs: ftok(char*, int)
	flagek: hozzáférési engedélyek és _IPC_CREAT | _IPC_EXCL
	méret: getpagesize() poz. szorzatai. pl: getpagesize()->4096, méret: 5 -> 4096. méret: 4100 -> 8192
shmat(shmid, NULL, 0): csatlakoztatja a folyamatot -> osztott memória kezdõcíme, shmaddr vagy -1
shmdt(shmaddr): leválasztja a folyamatot
shmctl(shmid, IPC_RMID, NULL): felszabadítja a memóriát és törli a bejegyzések közül

példa:
osztmem.c