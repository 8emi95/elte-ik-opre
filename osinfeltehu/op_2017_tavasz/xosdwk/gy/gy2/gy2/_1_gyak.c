// 2. gyakorlat. Jelz�sek
/*
	- mai graf oprendszerekben prog k�zben haszn�ljuk
	- esem�ny feliratkoz�skor haszn�ljuk (event helyett signal)
	- signal.h
	- definici�s konstansok: SIGTERM, SIGKILL => ezek eg�sz sz�mok, jelz�s azonos�t�k
	- mindegyik jelz�shez feliratkozhatunke egy v�grehajt� f�gv�nyt
	- signal elj�r�s: signal(SIGUSR1:melyik esem�nyre, fv)
	- sginal-t k�ldeni: kill(Pid, SIG...) process azon, signal id
	- esem�nykezel� f�gv�ny: void fv(int i){}..
	- pause() -> jelz�sre v�r� fv, a folyamatban egy tetsz�leges jelz�sig v�r
*/
// 1. Mintaf�jl: signal.c 
//		Signal, kill rendszerh�v�sok.
//		signal(SIGUSR1,handler); A handler v�grehajt�sa alatt a SIGUSR1 jelz�sek kezel�se tiltott, 
//		de csak a SIGUSR1!!! Teh�t 2 egym�s ut�ni SIGUSR1-b�l csak az els� �r c�lba. 
//		Ha a szign�lt RT szign�lra cser�lem (pl. SIGRTMIN), akkor azok c�lba �rnek! 
//		Ha a SIGUSR2 kezel�s�t is handler v�gzi, akkor SIGUSR1, SIGUSR2 egym�s ut�ni k�ld�se c�lba �r!!!!
//
//		A signal egy r�gebbi, egyszer�bb kezel�si m�d, �jabb kezel� fv a sigaction, ezt a k�vetkez� minta mutatja meg. 
//		A jelz�sekr�l egy �ltal�nos le�r�s itt is olvashat�: http://man7.org/linux/man-pages/man7/signal.7.html

// 2. Mintaf�jl: sigaction.c
//		Legt�bbsz�r signallal egyen�rt�k� (k�l�nbs�g, blokkolhat� jelz�sek be�ll�t�sa, 
//			egy adat tov�bb�t�si lehet�s�g �s SA_FLAGS. 
//			M�g a signal h�v�s eldobja az �ppen kezelt szign�lt a kezel� v�grehajt�sa alatt, 
//			addig a sigaction nem! A sigaction(SIGUSR1,sigaction act,sigaction oldact) h�v�s a sigaction 
//			strukt�r�ban v�rja kezel� f�ggv�ny megad�s�t �s bizonyos viselked�st adhatunk meg.
//			(oldact a r�gi ment�se, ez gyakran 0 �rt�ket kap)
//		A sigaction strukt�ra mez�i: 	
//			- sa_handler- a kezel� fv mez�je, mint signal-n�l
//			- sa_mask	- azon jelz�sek, melyeket blokkolni kell a handler v�grehajt�sa sor�n, 
//				sigemptyset(&sigact.sa_mask); // �res mask sigaddset(&sigact.sa_mask,SIGUSR1); // SIGUSR1 blokkol�s csak a handler alatt!!!
//
// 		M�g az al�bbi csoportkezel� makr�k haszn�lhat�k:
// 		sigfillset(&sigact.sa_mask); // minden szign�l a csoportban van 
// 		sigdelset(&sigact.sa_mask, SIGUSR1); // SIGUSR1 t�rl�se a csoportb�l
//					sa_flags 	- viselked�s megad�sa
// 		Ha sa_flags=SA_SIGINFO, akkor a
//					sa_sigaction	- 3 param�teres kezel�,SA_SIGINFO
// 		eset�n: void func(int signo, siginfo_t *info, void *context);alak�
// 		A siginfo_t a jelz�sr�l ad inform�ci�t, mez�i si_valami alak�ak, a fontosabb mez�k a k�vetkez�k: si_signo- a jelz�s sz�ma(SIGUSR1 stb.), si_code-inform�ci� a jelz�s k�ld�j�r�l, si_value- egy union, a si_value.sival_int egy eg�sz �rt�k, vagy a si_value.sival_ptr egy void* mutat�. Ezen kereszt�l k�ldhet�nk a jelz�ssel egy�tt egy adatot a kezel� f�ggv�nyhez! Ez az �rt�k pl. egy sigevent sigev.value mez�j�t kapja meg, ha egy posix �zenetsor �r�skor be�ll�tjuk, hogy �zenet �rkez�skor err�l egy jelz�s menjen. Erre l�tunk p�ld�t a k�s�bbi �zenetsor haszn�latn�l.
// 		Ezt a si_value mez�t a kill h�v�s nem haszn�lja. Ha ezt ki akarjuk haszn�lni akkor a sigqueue h�v�ssal kell jelz�st k�ldeni. Ennek alakja a k�vetkez�:
//			int sigqueue(pid_t pid, int sig, const union sigval value);
// 		Err�l a h�v�sr�l b�vebb le�r�st pl az al�bbi link ad: http://man7.org/linux/man-pages/man3/sigqueue.3.html
// 		Egy full manual, itt olvashat� a sigaction h�v�sr�l: http://man7.org/linux/man-pages/man2/sigaction.2.html

// 3. Mintaf�jl: sigprocmask.c 
//		A jelz�sek blokkolhat�ak, k�s�bb meg�rkeznek sigprocmask, pause() � jelz�sre v�r

//4. Mintaf�jl: setitimer.c 
//		Id�z�t�s: id�z�t� elindul�s�nak kezdete is be�ll�that�

#include <unistd.h> 	// alarm, pause, fork, getpid, exec, fcntl, sleep
#include <sys/types.h>	// wait, getpid
#include <sys/wait.h>	// wait
#include <stdlib.h> 	// system, rand(), srand()
#include <fcntl.h> 		// fcntl, open
#include <sys/stat.h> 	// open
#include <signal.h> 	// signal, kill, sigaction, sigemptyset, sigprocmask,
#include <string.h> 	// strsignal � jelz�s le�r�sa
#include <sys/time.h> 	// setitimer

main(){
	
}
