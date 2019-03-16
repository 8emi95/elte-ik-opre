#include <signal.h> /* Itt elérhető a szignállal kapcsolatos függvények */
#include <sys/time.h> /* időzítéssel kapcsolatos dolgok */
#include <time.h> /* timer_create, itimerspec */

void handler (int signo) /* Nincs handler beállítva */
void handler (int signo, siginfo_t *info, void *context) /* Be van állítva */

struct sigaction /* Egy szignál struktúrája */
sigaction(signum, &act, &oldact)

sigsuspend(&mask) /* A megadott maskkal meghatározhatjuk, hogy melyik szignálra várakozunk, addig felfüggeszti a többi szignált míg a várt szignál meg nem érkezik */

/* Valós idejű szignálok, FORDÍTÁSNÁL: gcc valami.c -lrt */

SIGRTMIN, SIGRTMAX /* Az e közé eső szignálok a real-time szignálok 
                      Minél alacsonyabb a száma a szignálnak, annál hamarabb lesz kiszolgálva */

raise(signal)

struct itimerval
struct timeval
settimer(időzítő, &újtimer, &régitimer) /* Beállítja az időzítőt */
gettimer(időzítő, &timer) /* Lekérdezi az időzítőt */

/* Union típus */

union a {
   típus1 adattag1; // int i
   típus2 adattag2; // char c
}

sigqueue(PID,signal,value) /* Lehet a szignállal értéket küldeni */
timer_create(clockid, &signalevent, &timerid) /* clockid: CLOCK_REALTIME, ...
                                                 signalevent: sigev_notify: SIGEV_NONE, SIGEV_SIGNAL
                                                              sigev_signo: ...
                                                              sigev_value: ...
                                                 timerid: timer_t */
