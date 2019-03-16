// 6. gyakorlat. Szemaforok
/*

1. szemafor.c: System V szemaforok
	#include <sys/sem.h>
	Létrehozása a semget paranccsal. Ez valójában egy szemafor családot hoz létre N szemaforral: int semid=semget(kulcs,N,IPC_CREAT|S_IRUSR|S_IWUSR)
	Ha -1 a semid, akkor sikertelen a léterhozás.
	A szemafor család i. szemaforjának beállítása: semctl(semid,i,SETVAL,szemafor_ertek)
	Ha a semctl -1-et ad vissza, sikertelen a beállítás. Ha 1 szemafor van a családban, N=1, akkor i csak 0 lehet (i az index szám a szemafor tömbben)
	Szemafor törlés: semctl(semid,0,IPC_RMID), a második paraméter (0) nem érdekes, a teljes szemafor családot törli!!!
	Szemafor mûvelet:
	muvelet.sem_num = 0; // szemafor index
	muvelet.sem_op  = 1; // 1 up, -1 down
	muvelet.sem_flg = 0; // várakozik ha kell, lehetne IPC_NOWAIT is!!
	semop(semid,&muvelet,1);
	A muvelet egy sembuf mutató, a semop harmadik paramétere megmondja, hogy muvelet tömbnek hány eleme van, hány szemafor mûveletet kell elvégezni!


2. pos_szemafor.c: Posix szemafor család
	A Posix szemafor utasítás család újabb, csak a 2.6-os kernelek után érhetõ el. Nem szemafor családot gyárt!
	#include <semaphore.h>
	Fordítás: cc –lrt pos_szemafor.c, az új SLE 12 SP1 alatt –pthread kapcsoló kell a –lrt helyett
	Létrehozása:  Nevesített sem_open(„alma”,..), a nem nevesítettet a sem_init hozza létre, elsõsorban szálakhoz kötõdik.
	sem_t *semid=sem_open(„alma”,O_CREAT,S_IRUSR|S_IWUSR,szemafor_ertek );
	if (semid==SEM_FAILED) perror("sem_open");
	Törlés: sem_unlink(semid);
	Down: sem_wait(semid);
	A sem_wait várakozik, ha a szemafor értéke 0. Létezik a sem_trywait(semid) is, ez ha a szemafor értéke 0 akkor nem vár, 
	hanem rögtön visszatér és beállítja errno értékét EAGAIN-re! A sem_timedwait(semid, timeout) forma vár szemafor 0 esetén 
	a timeoutban megadott ideig. Ha ezalatt nem lesz a szemafor szabad, akkor errno=ETIMEDOUT-ból láthatjuk , 
	hogy a szemafor még blokkolt, de a türelmünk elfogyott.
	Up: sem_post(semid)

*/

#include <unistd.h> 	// alarm, pause, fork, getpid, exec, fcntl, sleep
#include <sys/types.h>	// wait, getpid
#include <sys/wait.h>	// wait
#include <stdlib.h> 	// system, rand(), srand()
#include <fcntl.h> 		// fcntl, open
#include <sys/stat.h> 	// open
#include <signal.h> 	// signal, kill, sigaction, sigemptyset, sigprocmask,
#include <string.h> 	// strsignal – jelzés leírása
#include <sys/time.h> 	// setitimer
#include <sys/sem.h>	// semaphore

main() {

}