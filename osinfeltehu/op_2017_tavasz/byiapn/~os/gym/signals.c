1) sigset
<signal.h>
sigset_t típus : sigset_t sigset
inicializációk:
sigemptyset(&sigset) mindent átengedõ üres készlet
mûveletek:
sigfillset(&sigset) összes szignál hozzáadása a készlethez
sigaddset(&sigset, {szignál}) szignál hozzáadása a készlethez
sigdelset(&sigset, {szignál}) szignál törlése a készletbõl
sigprocmask(<mûvelet> , &sigset, &oldset) -> 0, ha siker | -1, ha nem
	mûveletek: 	SIG_BLOCK : hozzáveszi a maszkhoz a sigset kizárásait is(!)
			SIG_UNBLOCK : elveszi a maszkból a sigset kizárásait
			SIG_SETMASK : átállítja a teljes maszkot a sigset kizárásaira
	&sigset: új sigset
	&oldset: biztonsági mentés az elõtte levõ állapotról, lehet NULL

2) sigaction
<signal.h>
struct sigaction
	sa_handler : handler 	{SIG_DFL | SIG_IGN}
	sa_mask : sigset_t	
	sa_flags : 0 : alapviselkedés | SA_SIGINFO : ha nincs beállítva: void handler(int signo)
						     ha be van állítva: void handler(int signo, siginfo_t* info, void *context)
sigaction(signum, &act, &oldact)
sigsuspend(&mask) : lecseréli a maszkot az adott maszkra és vár egy abban nem szereplõ(nem blokkolt) szignálra

3) valós idejû szignálok / Real Time signals
A SIGRTMIN, SIGRTMAX konstansok közé esõ számú szignálok valós idejû szignálok.
(!)Fordításukhoz szükséges a -lrt kapcsoló(!)
A valós idejû szignálok nem írják felül egymást várakozáskor.

4) raise
<signal.h>
raise(signal) : önmagának küld szignált, handlerbõl hívva is mûködik - a handler befejezése után kezeli le a program az új szignált

5) idõzítõk
<unistd.h>
alarm(delay(sec)) -> SIGALARM
<sys/time.h> (!)teljesen más mint a sima time.h(!)
struct itimerval
	_it_interval : timeval
	_it_value : timeval
struct timeval
	_tv_sec : seconds
	_tv_usec : microseconds
mûveletek:
itimerval timer, oldtimer
settimer(<idõzítõ>, &timer, &oldtimer) -> 0, ha siker | -1, ha nem
	idõzítõk:	ITIMER_REAL : valós idejû idõzítõ -> SIGALARM-ot küld
			ITIMER_VIRTUAL : a végrehajtási idõt méri csak -> SIGVTALARM-ot küld
			ITIMER_PROOF -> SIGPROOF
gettimer(<idõzítõ>, itimerval &timer)

Hogy állítja le a handler settimer a programot?
Mit csinál a handler settimer 18-19. sora?
Miért írja ki a "0 seconds"-öket.

<time.h>
timer_create(clockid, &signalevent, &timerid) -> itimerspec
	clockid : CLOCK_REALTIME
	sigevent:
		sigev_notify : SIGEV_NONE | SIGEV_SIGNAL
		sigev_signo
		sigev_value
	timerid : timer_t
itimerspec timer, oldtimer
timer_settime(timerid, flags: NULL, &timer, &oldtimer)
timer_gettime(timerid, &act_value)

6) unió
union a {
	típus1 adattag1;
	típus2 adattag2;
} : egy memória területen belül tárolja a két adattagot.

pl: int i(4B) és char c(1B) esetén
 c
|X|X|X|X|_|_|_|_|
 i i i i

7) info küldése szignállal
<signal.h>
siginfo_t
	si_code
	si_pid
	si_value : -int: info -> si_value.sival_int
		   -ptr: info -> si_value.sival_ptr

sigqueue(PID, signal, value)