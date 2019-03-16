OSZTOTT MEMÓRIA



-> OM LÉTREHOZÁSA:

	key_t kulcs;
	int sh_mem_id;

	kulcs = ftok(argv[0],1);
		//a parancs nevevel es az 1 verzio szammal kulcs generalas
	sh_mem_id = shmget(kulcs, MEMSIZE, IPC_CREAT|S_IRUSR|S_IWUSR);
		//kell hozzá: #define MEMSIZE 1024
	Vagy:
	sh_mem_id = shmget(kulcs, 500, IPC_CREAT|S_IRUSR|S_IWUSR);
		//osztott memoria letrehozasa, irasra olvasasra, 500 bajt mérettel
	    //kulcs helyett lehet IPC_CREAT
	
	
	
-> OM "LEKÉRDEZÉSE" VÁLTOZÓBA (CSATLAKOZÁS):

	char *s;
	s = shmat(sh_mem_id, NULL, 0);
		//a 2. parameter akkor kell, ha sajat cimhez akarjuk illeszteni
		//a 3. paraméter lehet SHM_RDONLY, ekkor csak read van
		//az shmat(...) egyébként void*-ot ad vissza, bárminek használhatom az OM-et
    
	   
   
-> OM ELENGEDÉSE:

Adott:
- char* s  -  OM-es változó

   shmdt(s);
   
   
   
-> OM TÖRLÉSE

Adott:
- int sh_mem_id  -  OM azonosító

	shmctl(sh_mem_id, IPC_RMID, NULL);
		//IPC_RMID- torolni akarjuk a memoriat, ekkor nem kell 3. parameter
		//IPC_STAT- osztott memoria adatlekerdezes a 3. parameterbe,
		//ami shmid_ds struct tipusu mutato