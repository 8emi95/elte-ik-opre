//// LINK with -lrt 
////send addional data with a signal

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/time.h>
#include <time.h>
  
timer_t t_id;
int count=0; 
void handler(int signumber){
  printf("Timer expired %i. time\n",count);
  if (count++==5){timer_delete(t_id);raise(SIGKILL);}
}

int main(){

  struct sigaction sigact;
  signal(SIGTERM,handler);  
  
  struct sigevent myevent; //sigevent for creating a timer
  myevent.sigev_signo=SIGTERM; //any signal 
  myevent.sigev_notify=SIGEV_SIGNAL;
  
  
  int bad=timer_create(CLOCK_REALTIME, &myevent,&t_id);
  if (bad!=0) {perror("Error\n");exit(EXIT_FAILURE);}
  
  struct itimerspec mytimerspec;
  mytimerspec.it_value.tv_sec=1;
  mytimerspec.it_interval.tv_sec=1;
  mytimerspec.it_value.tv_nsec=0;
  mytimerspec.it_interval.tv_nsec=0;

  if (timer_settime(t_id,0,&mytimerspec,NULL)<0) {perror("SETtimer error\n"); exit(EXIT_FAILURE);};
  printf("Send a SIGTERM by the timer in 1 sec\n");
  while (1);
  
  printf("\n---------------------------------\n");  
  return 0;
}