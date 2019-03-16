#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

void szignal(int s)
{
	signal(SIGALRM,szignal);
	printf("Megjött az ALARM jelzés, értéke: %d\n",s);
 struct itimerval dt;
 dt.it_interval.tv_sec=0;
 dt.it_interval.tv_usec=0;
 dt.it_value.tv_sec=10;
 dt.it_value.tv_usec=0;
 setitimer(ITIMER_REAL,&dt,0);
}
void kilep(int k)
{
 printf("\nProgram vége!\n");
 exit(0);
}
int main()
{
 /* ez megjegyzes */
 // ez egy egysoros megjegyzes
 printf("Hajra Fradi!\n");
 struct itimerval dt;
 dt.it_interval.tv_sec=0;
 dt.it_interval.tv_usec=0;
 dt.it_value.tv_sec=10;
 dt.it_value.tv_usec=0;
 setitimer(ITIMER_REAL,&dt,0);
 printf("Timer elindult!\n");
signal(SIGALRM,szignal);
signal(SIGINT,kilep);
 printf("Jelzések beállítva!\n");
 //alarm(4); // ez nem az igazi
// sleep(5);
 while (1);
 printf("Vége");
 return 0;
}
