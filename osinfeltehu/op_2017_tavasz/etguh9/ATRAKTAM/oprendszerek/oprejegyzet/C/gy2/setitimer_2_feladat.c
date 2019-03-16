#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

#include <time.h>


int count=0;
struct itimerval timer;

time_t now;
struct tm * timeinfo;

void handlersetitimer(int signalnumber)
{

//  time_t rawtime;
//  time(&rawtime);

  time(&now);
  timeinfo = localtime(&now);
  printf("%s",asctime(timeinfo));
}

int main(){
 struct sigaction sigact;
 sigemptyset(&sigact.sa_mask); //no blocked signals only the one, which arrives
 sigact.sa_handler=handlersetitimer;
 sigact.sa_flags=0; //no special behaviour
 sigaction(SIGALRM,&sigact,NULL); //an alarm signal is set

 timer.it_interval.tv_sec=1;
 timer.it_interval.tv_usec=0;
 timer.it_value.tv_sec=1;
 timer.it_value.tv_usec=0;       

 setitimer(ITIMER_REAL,&timer,NULL ); //result = 0, if it is good  
  //when expires, a signal will be sent to the process, and it restarts
  //1. parameter ITIMER_REAL - real time, ITIMER_VIRTUAL (during process execution)
  //2. parameter timer 
  //3. old timer value
 struct itimerval expires;
 while (1){
  ;
  sleep(1); //be careful with sleep
  getitimer(ITIMER_REAL,&expires);//reads the remaining time
  //printf("\tTimer will be expired after %i seconds\n", expires.it_value.tv_sec);
 }
 
 return 0;
}