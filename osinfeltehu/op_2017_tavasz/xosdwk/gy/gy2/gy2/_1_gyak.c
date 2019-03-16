// 2. gyakorlat. Jelzések
/*
	- mai graf oprendszerekben prog közben használjuk
	- esemény feliratkozáskor használjuk (event helyett signal)
	- signal.h
	- definiciós konstansok: SIGTERM, SIGKILL => ezek egész számok, jelzés azonosítók
	- mindegyik jelzéshez feliratkozhatunke egy végrehajtó fügvényt
	- signal eljárás: signal(SIGUSR1:melyik eseményre, fv)
	- sginal-t küldeni: kill(Pid, SIG...) process azon, signal id
	- eseménykezelõ fügvény: void fv(int i){}..
	- pause() -> jelzésre váró fv, a folyamatban egy tetszõleges jelzésig vár
*/
// 1. Mintafájl: signal.c 
//		Signal, kill rendszerhívások.
//		signal(SIGUSR1,handler); A handler végrehajtása alatt a SIGUSR1 jelzések kezelése tiltott, 
//		de csak a SIGUSR1!!! Tehát 2 egymás utáni SIGUSR1-bõl csak az elsõ ér célba. 
//		Ha a szignált RT szignálra cserélem (pl. SIGRTMIN), akkor azok célba érnek! 
//		Ha a SIGUSR2 kezelését is handler végzi, akkor SIGUSR1, SIGUSR2 egymás utáni küldése célba ér!!!!
//
//		A signal egy régebbi, egyszerûbb kezelési mód, újabb kezelõ fv a sigaction, ezt a következõ minta mutatja meg. 
//		A jelzésekrõl egy általános leírás itt is olvasható: http://man7.org/linux/man-pages/man7/signal.7.html

// 2. Mintafájl: sigaction.c
//		Legtöbbször signallal egyenértékû (különbség, blokkolható jelzések beállítása, 
//			egy adat továbbítási lehetõség és SA_FLAGS. 
//			Míg a signal hívás eldobja az éppen kezelt szignált a kezelõ végrehajtása alatt, 
//			addig a sigaction nem! A sigaction(SIGUSR1,sigaction act,sigaction oldact) hívás a sigaction 
//			struktúrában várja kezelõ függvény megadását és bizonyos viselkedést adhatunk meg.
//			(oldact a régi mentése, ez gyakran 0 értéket kap)
//		A sigaction struktúra mezõi: 	
//			- sa_handler- a kezelõ fv mezõje, mint signal-nál
//			- sa_mask	- azon jelzések, melyeket blokkolni kell a handler végrehajtása során, 
//				sigemptyset(&sigact.sa_mask); // üres mask sigaddset(&sigact.sa_mask,SIGUSR1); // SIGUSR1 blokkolás csak a handler alatt!!!
//
// 		Még az alábbi csoportkezelõ makrók használhatók:
// 		sigfillset(&sigact.sa_mask); // minden szignál a csoportban van 
// 		sigdelset(&sigact.sa_mask, SIGUSR1); // SIGUSR1 törlése a csoportból
//					sa_flags 	- viselkedés megadása
// 		Ha sa_flags=SA_SIGINFO, akkor a
//					sa_sigaction	- 3 paraméteres kezelõ,SA_SIGINFO
// 		esetén: void func(int signo, siginfo_t *info, void *context);alakú
// 		A siginfo_t a jelzésrõl ad információt, mezõi si_valami alakúak, a fontosabb mezõk a következõk: si_signo- a jelzés száma(SIGUSR1 stb.), si_code-információ a jelzés küldõjérõl, si_value- egy union, a si_value.sival_int egy egész érték, vagy a si_value.sival_ptr egy void* mutató. Ezen keresztül küldhetünk a jelzéssel együtt egy adatot a kezelõ függvényhez! Ez az érték pl. egy sigevent sigev.value mezõjét kapja meg, ha egy posix üzenetsor íráskor beállítjuk, hogy üzenet érkezéskor errõl egy jelzés menjen. Erre látunk példát a késõbbi üzenetsor használatnál.
// 		Ezt a si_value mezõt a kill hívás nem használja. Ha ezt ki akarjuk használni akkor a sigqueue hívással kell jelzést küldeni. Ennek alakja a következõ:
//			int sigqueue(pid_t pid, int sig, const union sigval value);
// 		Errõl a hívásról bõvebb leírást pl az alábbi link ad: http://man7.org/linux/man-pages/man3/sigqueue.3.html
// 		Egy full manual, itt olvasható a sigaction hívásról: http://man7.org/linux/man-pages/man2/sigaction.2.html

// 3. Mintafájl: sigprocmask.c 
//		A jelzések blokkolhatóak, késõbb megérkeznek sigprocmask, pause() – jelzésre vár

//4. Mintafájl: setitimer.c 
//		Idõzítés: idõzítõ elindulásának kezdete is beállítható

#include <unistd.h> 	// alarm, pause, fork, getpid, exec, fcntl, sleep
#include <sys/types.h>	// wait, getpid
#include <sys/wait.h>	// wait
#include <stdlib.h> 	// system, rand(), srand()
#include <fcntl.h> 		// fcntl, open
#include <sys/stat.h> 	// open
#include <signal.h> 	// signal, kill, sigaction, sigemptyset, sigprocmask,
#include <string.h> 	// strsignal – jelzés leírása
#include <sys/time.h> 	// setitimer

main(){
	
}
