#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>


int count=0;
struct itimerval timer;

void handlersetitimer(int signalnumber)
{ count++;
  printf("\nThe program was alarmed by setitimer the %i. time\n",count);
  if (count==5) {
    timer.it_interval.tv_sec=0; /* it will be stopped */
    timer.it_interval.tv_usec=0;
    
    raise(SIGTERM); //it will stop process
    // raise a signal and send it to the process itself
  }
}
int main(){

 signal(SIGALRM,handlersetitimer);
 timer.it_interval.tv_sec=3; /* it will be repeated after 3 seconds */
 timer.it_interval.tv_usec=0;
 timer.it_value.tv_sec=1;    /* first alarm will start after tv_sec */
 timer.it_value.tv_usec=0;       

 setitimer(ITIMER_REAL,&timer,NULL ); //result = 0, if it is good  
 printf("It will start after 1 sec\n");
  //when expires, a signal will be sent to the process, and it restarts
  //1. parameter ITIMER_REAL - real time, ITIMER_VIRTUAL (during process execution)
  //2. parameter timer 
  //3. old timer value
 // getitimer() - you can read the data of the timer 
 while (1){
  sleep(1); //be careful with sleep
  printf("."); fflush(stdout);
 }
 
 return 0;
}