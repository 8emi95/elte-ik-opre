#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

int count=0;
struct itimerval timer;

void handleralarm(int signalnumber){
 printf("The program was alarmed by alarm function\n");
 
}
void handlersetitimer(int signalnumber)
{ count++;
  printf("The program was alarmed by setitimer the %i. time\n",count);
  if (count==5) {
    timer.it_interval.tv_sec=0; /* it will be stopped */
    timer.it_interval.tv_usec=0;
    
    raise(SIGTERM); //it will stop process
    // raise a signal and send it to the process itself
  }
}
int main(){
 //1. trial of creating a timer by alarm function

 timer.it_interval.tv_sec=1; /* it will be repeated after 3 seconds */
 timer.it_interval.tv_usec=0;
 timer.it_value.tv_sec=1;    /* remaining time till expiration */
 timer.it_value.tv_usec=0;       

 setitimer(ITIMER_REAL,&timer,NULL );
 printf("%s",ctime(timer));
 
 return 0;
}