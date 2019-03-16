1) sigset
<signal.h>
sigset_t t�pus : sigset_t sigset
inicializ�ci�k:
sigemptyset(&sigset) mindent �tenged� �res k�szlet
m�veletek:
sigfillset(&sigset) �sszes szign�l hozz�ad�sa a k�szlethez
sigaddset(&sigset, {szign�l}) szign�l hozz�ad�sa a k�szlethez
sigdelset(&sigset, {szign�l}) szign�l t�rl�se a k�szletb�l
sigprocmask(<m�velet> , &sigset, &oldset) -> 0, ha siker | -1, ha nem
	m�veletek: 	SIG_BLOCK : hozz�veszi a maszkhoz a sigset kiz�r�sait is(!)
			SIG_UNBLOCK : elveszi a maszkb�l a sigset kiz�r�sait
			SIG_SETMASK : �t�ll�tja a teljes maszkot a sigset kiz�r�saira
	&sigset: �j sigset
	&oldset: biztons�gi ment�s az el�tte lev� �llapotr�l, lehet NULL

2) sigaction
<signal.h>
struct sigaction
	sa_handler : handler 	{SIG_DFL | SIG_IGN}
	sa_mask : sigset_t	
	sa_flags : 0 : alapviselked�s | SA_SIGINFO : ha nincs be�ll�tva: void handler(int signo)
						     ha be van �ll�tva: void handler(int signo, siginfo_t* info, void *context)
sigaction(signum, &act, &oldact)
sigsuspend(&mask) : lecser�li a maszkot az adott maszkra �s v�r egy abban nem szerepl�(nem blokkolt) szign�lra

3) val�s idej� szign�lok / Real Time signals
A SIGRTMIN, SIGRTMAX konstansok k�z� es� sz�m� szign�lok val�s idej� szign�lok.
(!)Ford�t�sukhoz sz�ks�ges a -lrt kapcsol�(!)
A val�s idej� szign�lok nem �rj�k fel�l egym�st v�rakoz�skor.

4) raise
<signal.h>
raise(signal) : �nmag�nak k�ld szign�lt, handlerb�l h�vva is m�k�dik - a handler befejez�se ut�n kezeli le a program az �j szign�lt

5) id�z�t�k
<unistd.h>
alarm(delay(sec)) -> SIGALARM
<sys/time.h> (!)teljesen m�s mint a sima time.h(!)
struct itimerval
	_it_interval : timeval
	_it_value : timeval
struct timeval
	_tv_sec : seconds
	_tv_usec : microseconds
m�veletek:
itimerval timer, oldtimer
settimer(<id�z�t�>, &timer, &oldtimer) -> 0, ha siker | -1, ha nem
	id�z�t�k:	ITIMER_REAL : val�s idej� id�z�t� -> SIGALARM-ot k�ld
			ITIMER_VIRTUAL : a v�grehajt�si id�t m�ri csak -> SIGVTALARM-ot k�ld
			ITIMER_PROOF -> SIGPROOF
gettimer(<id�z�t�>, itimerval &timer)

Hogy �ll�tja le a handler settimer a programot?
Mit csin�l a handler settimer 18-19. sora?
Mi�rt �rja ki a "0 seconds"-�ket.

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

6) uni�
union a {
	t�pus1 adattag1;
	t�pus2 adattag2;
} : egy mem�ria ter�leten bel�l t�rolja a k�t adattagot.

pl: int i(4B) �s char c(1B) eset�n
 c
|X|X|X|X|_|_|_|_|
 i i i i

7) info k�ld�se szign�llal
<signal.h>
siginfo_t
	si_code
	si_pid
	si_value : -int: info -> si_value.sival_int
		   -ptr: info -> si_value.sival_ptr

sigqueue(PID, signal, value)