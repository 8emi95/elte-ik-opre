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

/* Most n�zz�nk egy egyszer� p�ld�t a megosztott mem�riahaszn�latr�l. A p�ldaprogramban a folyamat
l�trehoz egy megosztott mem�ri�t, csatlakozik hozz�, majd a fork h�v�ssal l�trehoz egy gyerek folyamatot.
A sz�l� folyamat bele�r egy sz�veget a megosztott mem�ri�ba, a gyerek folyamat pedig kiolvassa azt. A
kiolvas�s el�tt a gyerek v�r 1 m�sodpercet, hogy a sz�l� azt be tudja �rni. A p�ld�ban semmit nem tesz�nk a
versenyhelyzet elker�l�s�re, csak abban b�zunk, hogy 1 m�sodperc alatt m�r be�rta az adatokat a sz�l�. */

int main (int argc,char* argv[]) {
	pid_t child;
	key_t kulcs;
	int sh_mem_id;
	char *s;
	/*A l�trehoz�skor adni kell egy kulcsot,
	amelyet az ftok f�ggv�nnyel gener�lhatunk. */ 
	kulcs=ftok(argv[0],1);

	/*A MEMSIZE m�ret� megosztott mem�ria l�trehoz�s,
	olvas�si �s �r�si jogosults�ggal a felhaszn�l� r�sz�re. */
	sh_mem_id=shmget(kulcs,MEMSIZE,IPC_CREAT|S_IRUSR|S_IWUSR);
	
	/*Csatlakoz�s a megosztott mem�ri�hoz.
	Itt kapott mem�ria c�met. */
	s = shmat(sh_mem_id,NULL,0);
	child = fork();
	if(child>0){

		char buffer[] = "Hello!\n";

		strcpy(s,buffer);
		
		/*Lev�l�s a megosztott mem�ri�r�l. */
		shmdt(s);
		wait(NULL);
		/* Megv�rja m�g v�get �r a gyerek
		folyamat majd t�rli a megosztott mem�ri�t. */
		shmctl(sh_mem_id,IPC_RMID,NULL);
	} else if ( child == 0 ) { 
		sleep(1);
		printf("gyerek: %s",s);
		shmdt(s);
	}
	return 0;
} 