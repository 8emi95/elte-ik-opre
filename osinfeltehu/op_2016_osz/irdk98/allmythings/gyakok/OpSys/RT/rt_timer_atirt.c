       #include <stdlib.h>
       #include <unistd.h>
       #include <stdio.h>
       #include <signal.h>
       #include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>
#include <sys/mman.h>

       #define CLOCKID CLOCK_REALTIME
       
       #define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                               } while (0)

       
      struct itimerspec its,its2;
      timer_t timerid, timerid2;
      volatile sig_atomic_t db=0,c=0;      
      void  handler(int sig,siginfo_t *si, void * uc)
       {     printf("megkapta %i\n",c);
             if (c++==0){
               if (timer_settime(timerid, 0, &its, NULL) == -1)
                   errExit("timer_settime");
             }else {
                timer_delete(timerid);
             }      

       }
       void  handler_timer(int sig,siginfo_t *si, void * uc)
       {
            db++;

       }
       int
       main(int argc, char *argv[])
       {
 	
 	pid_t child;
   	
    	
           struct sigevent sev,sev2;
           long long freq_nanosecs;
           sigset_t mask;
           struct sigaction sa,sa2;

           db=0;        
           sa.sa_flags = 0; //SA_RESTART;
           sa.sa_sigaction=handler_timer;
           sigemptyset(&sa.sa_mask);
           sigaddset(&sa.sa_mask,SIGRTMIN); //for concurency to avoid to change db value 

           if (sigaction(SIGRTMIN, &sa, NULL) == -1)
               errExit("sigaction");
           sa2.sa_flags = 0; //SA_RESTART;
           sa2.sa_sigaction=handler;
           sigemptyset(&sa2.sa_mask);
           sigaddset(&sa2.sa_mask,SIGRTMAX); //for concurency to avoid to change db value 

           if (sigaction(SIGRTMAX, &sa, NULL) == -1)
               errExit("sigaction");
           if (sigaction(SIGRTMIN, &sa2, NULL) == -1)
               errExit("sigaction");

           /* Create the timer */

           sev.sigev_notify = SIGEV_SIGNAL;
            sev.sigev_signo = SIGRTMIN;
             sev.sigev_value.sival_ptr = &timerid;
           if (timer_create(CLOCKID, &sev, &timerid) == -1)
               errExit("timer_create");
               printf("atirtban timer ID is 0x%lx\n", (long) timerid);

           /* Start the timer */ 

           its.it_value.tv_sec = 0;  //freq_nanosecs / 1000000000;
           its.it_value.tv_nsec = 1000 ;//  freq_nanosecs % 1000000000;
           its.it_interval.tv_sec = 0;
           its.it_interval.tv_nsec = 1000;
           while(1);   
           printf("%i\n",db);          
        
           exit(EXIT_SUCCESS);
       }