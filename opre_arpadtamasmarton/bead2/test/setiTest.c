#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <time.h>
//sleep() may be implemented using SIGALRM, mixing calls alarm() and
//sleep() is a bad idea
//setitimer and alarm let the process interrupt itself in the future
//each process has three independent interval timers available:
//a real-time timer that counts processor time
void handler();
int main()
{
struct itimerval interval;
struct timespec sleep;
struct timeval t;
sleep.tv_sec = 15;
sleep.tv_nsec = 0;
interval.it_interval.tv_sec = 5;
interval.it_interval.tv_usec = 0;
interval.it_value.tv_sec = 1;
interval.it_value.tv_usec = 0;
t.tv_sec = 30;
t.tv_usec = 0;
//signal(SIGVTALRM, handler);
setitimer(ITIMER_VIRTUAL, &interval, NULL);
#if 0
while(1) {
//nanosleep(&sleep, NULL);
//sleep(10);
select(0, NULL, NULL, NULL, &t);
printf("bye\n");
}
#else
getchar();
#endif
//pause to quit
#if 0
cout >> "Enter 1 to Exit \n";
cin >> m;
system("pause");
return 0;
getchar();
#endif
return 0;
}
void handler(int signal)
{
printf("HI\n");
return;
}
