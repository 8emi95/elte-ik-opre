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


// union sigval {
   // int   sival_int;  // the data is int, sent by a signal
   // void *sival_ptr;  // or the data is a pointer!
// };


void handler(int signumber,siginfo_t* info,void* nonused)
{
	printf("Signal with number %i has arrived\n",signumber);
	switch (info->si_code)
	{
		case SI_QUEUE:
			printf("It was sent by a sigqueue, sending process (PID %i)\n",info->si_pid);
			printf("Additional value: %i\n",info->si_value.sival_int);
			break;
		default:
			printf("It was sent by something else \n");
	} 
}


//MAIN:

	struct sigaction sigact;  
	sigact.sa_sigaction=handler; //instead of sa_handler, we use the 3 parameter version
	sigemptyset(&sigact.sa_mask); 
	sigact.sa_flags=SA_SIGINFO; //we need to set the siginfo flag 
	sigaction(SIGTERM,&sigact,NULL);
  
  
	//sendig an integer as an additional data :
	union sigval s_value_int={35};
	sigqueue(getppid(),SIGTERM,s_value_int); //just the same as kill function, but we can send additional data too