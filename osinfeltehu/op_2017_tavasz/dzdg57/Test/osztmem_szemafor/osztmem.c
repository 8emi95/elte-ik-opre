#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#define MEMSIZE 1024

// http://ntibi.web.elte.hu/elte/2010-2011-2/opr/osztmem_szemafor.pdf

/* Most nézzünk egy egyszerû példát a megosztott memóriahasználatról. A példaprogramban a folyamat
létrehoz egy megosztott memóriát, csatlakozik hozzá, majd a fork hívással létrehoz egy gyerek folyamatot.
A szülõ folyamat beleír egy szöveget a megosztott memóriába, a gyerek folyamat pedig kiolvassa azt. A
kiolvasás elõtt a gyerek vár 1 másodpercet, hogy a szülõ azt be tudja írni. A példában semmit nem teszünk a
versenyhelyzet elkerülésére, csak abban bízunk, hogy 1 másodperc alatt már beírta az adatokat a szülõ. */

int main (int argc,char* argv[]) {
	pid_t child;
	key_t kulcs;
	int sh_mem_id;
	char *s;
	/*A létrehozáskor adni kell egy kulcsot,
	amelyet az ftok függvénnyel generálhatunk. */ 
	kulcs=ftok(argv[0],1);

	/*A MEMSIZE méretû megosztott memória létrehozás,
	olvasási és írási jogosultsággal a felhasználó részére. */
	sh_mem_id=shmget(kulcs,MEMSIZE,IPC_CREAT|S_IRUSR|S_IWUSR);
	
	/*Csatlakozás a megosztott memóriához.
	Itt kapott memória címet. */
	s = shmat(sh_mem_id,NULL,0);
	child = fork();
	if(child>0){

		char buffer[] = "Hello!\n";

		strcpy(s,buffer);
		
		/*Leválás a megosztott memóriáról. */
		shmdt(s);
		wait(NULL);
		/* Megvárja míg véget ér a gyerek
		folyamat majd törli a megosztott memóriát. */
		shmctl(sh_mem_id,IPC_RMID,NULL);
	} else if ( child == 0 ) { 
		sleep(1);
		printf("gyerek: %s",s);
		shmdt(s);
	}
	return 0;
} 