// 6. gyakorlat. Szemaforok
/*

1. szemafor.c: System V szemaforok
	#include <sys/sem.h>
	L�trehoz�sa a semget paranccsal. Ez val�j�ban egy szemafor csal�dot hoz l�tre N szemaforral: int semid=semget(kulcs,N,IPC_CREAT|S_IRUSR|S_IWUSR)
	Ha -1 a semid, akkor sikertelen a l�terhoz�s.
	A szemafor csal�d i. szemaforj�nak be�ll�t�sa: semctl(semid,i,SETVAL,szemafor_ertek)
	Ha a semctl -1-et ad vissza, sikertelen a be�ll�t�s. Ha 1 szemafor van a csal�dban, N=1, akkor i csak 0 lehet (i az index sz�m a szemafor t�mbben)
	Szemafor t�rl�s: semctl(semid,0,IPC_RMID), a m�sodik param�ter (0) nem �rdekes, a teljes szemafor csal�dot t�rli!!!
	Szemafor m�velet:
	muvelet.sem_num = 0; // szemafor index
	muvelet.sem_op  = 1; // 1 up, -1 down
	muvelet.sem_flg = 0; // v�rakozik ha kell, lehetne IPC_NOWAIT is!!
	semop(semid,&muvelet,1);
	A muvelet egy sembuf mutat�, a semop harmadik param�tere megmondja, hogy muvelet t�mbnek h�ny eleme van, h�ny szemafor m�veletet kell elv�gezni!


2. pos_szemafor.c: Posix szemafor csal�d
	A Posix szemafor utas�t�s csal�d �jabb, csak a 2.6-os kernelek ut�n �rhet� el. Nem szemafor csal�dot gy�rt!
	#include <semaphore.h>
	Ford�t�s: cc �lrt pos_szemafor.c, az �j SLE 12 SP1 alatt �pthread kapcsol� kell a �lrt helyett
	L�trehoz�sa:  Neves�tett sem_open(�alma�,..), a nem neves�tettet a sem_init hozza l�tre, els�sorban sz�lakhoz k�t�dik.
	sem_t *semid=sem_open(�alma�,O_CREAT,S_IRUSR|S_IWUSR,szemafor_ertek );
	if (semid==SEM_FAILED) perror("sem_open");
	T�rl�s: sem_unlink(semid);
	Down: sem_wait(semid);
	A sem_wait v�rakozik, ha a szemafor �rt�ke 0. L�tezik a sem_trywait(semid) is, ez ha a szemafor �rt�ke 0 akkor nem v�r, 
	hanem r�gt�n visszat�r �s be�ll�tja errno �rt�k�t EAGAIN-re! A sem_timedwait(semid, timeout) forma v�r szemafor 0 eset�n 
	a timeoutban megadott ideig. Ha ezalatt nem lesz a szemafor szabad, akkor errno=ETIMEDOUT-b�l l�thatjuk , 
	hogy a szemafor m�g blokkolt, de a t�relm�nk elfogyott.
	Up: sem_post(semid)

*/

#include <unistd.h> 	// alarm, pause, fork, getpid, exec, fcntl, sleep
#include <sys/types.h>	// wait, getpid
#include <sys/wait.h>	// wait
#include <stdlib.h> 	// system, rand(), srand()
#include <fcntl.h> 		// fcntl, open
#include <sys/stat.h> 	// open
#include <signal.h> 	// signal, kill, sigaction, sigemptyset, sigprocmask,
#include <string.h> 	// strsignal � jelz�s le�r�sa
#include <sys/time.h> 	// setitimer
#include <sys/sem.h>	// semaphore

main() {

}