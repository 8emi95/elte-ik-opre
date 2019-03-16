#include <signal.h>
#include <unistd.h>
#include <stdio.h>

int done = 0;

void sigHandler(int signo)
{
   if (signo == SIGINT)
   {
      done = 1;
   }
}

int main(void)
{
   struct sigaction sigact;

   sigemptyset(&sigact.sa_mask);

   sigact.sa_flags   = SA_SIGINFO;
   sigact.sa_handler = sigHandler;

   sigaction(SIGINT, &sigact, 0);

   printf("Press ctrl-c to quit...\n");

   while (!done) sleep(1);

   printf("all done!\n");

   return 0;
}
