#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>


void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  
  int i =0;
  
  for (i; i<5; i++){
	  
	   printf("szamolok, %d\n", i);
	  
	  
  }
  // SIGTERM 15
  //SIGUSR1 10
}

int main(){

  struct sigaction sigact;
  sigact.sa_handler=handler; //SIG_DFL,SIG_IGN
  sigemptyset(&sigact.sa_mask); //during execution of handler these signals will be blocked plus the signal    
  //now only the arriving signal, SIGTERM will be blocked
  sigact.sa_flags=0; //nothing special behaviour
  sigaction(SIGTERM,&sigact,NULL); // küldö oldal (? beállítása)
  sigaction(SIGUSR1,&sigact,NULL);
  sigaction(SIGUSR2,&sigact,NULL);
  //1. parameter the signal number
  //2. parameter the new sigaction with handler and blocked signals during the execution of handler (sa_mask) and a 
  //special sa_flags - it change the behavior of signal, 
  //e.g. SIGNOCLDSTOP - after the child process ended it won't send a signal to the parent 
  //3. parameter - &old sigset or NULL. 
  //If there is a variable, the function will fill with the value of formerly set sigset
  pid_t child=fork();
  if (child>0)
  {
    sigset_t sigset; // halmaz, milyen signalokra legyen majd cselekmény a FOGADÓ oldalra.
    sigfillset(&sigset); // fillset makróval feltöltjük az összes létező signállal
    sigdelset(&sigset,SIGTERM); // a SIGTERM signal kiszedjük belőlle.
   sigsuspend(&sigset);// kényelmes várakozás pause() helyett.  a sigsuspend addig meg amig a sigsetben lévő szignálok jönnek
	
	int i =0;
	int k =0;
	  sigsuspend(&sigset);
	
    // like pause() - except it waits only for signals not given in sigset
    //others will be blocked
    printf("The program comes back from suspending\n");
    int status;
    wait(&status);
    printf("Parent process ended\n");
  }
  else 
  {
    printf("Waits 3 seconds, then send a SIGUSR %i signal (it is not waited for by suspend) - so the suspend continues waiting\n", SIGUSR1);
   // sleep(1);
    kill(getppid(),SIGTERM); //benne van a sigfillsetben, nem regál rá, megfogja
  //  printf("Waits 3 seconds, then send a SIGTERM %i signal (it is waited for by suspend)\n",SIGUSR2);
//	sleep(1);
	 kill(getppid(),SIGTERM); //benne van a sigfillsetben, nem regál rá, megfogja
//    printf("Waits 3 seconds, then send a SIGTERM %i signal (it is waited for by suspend)\n",SIGTERM);
  //  sleep(2);
    kill(getppid(),SIGTERM); // kitöröltük a sigfilsertből,  szval lefut a handler.

  //  kill(getppid(),SIGTERM); // kitöröltük a sigfilsertből,  szval lefut a handler.
	//kill(getppid(),SIGTERM); // kitöröltük a sigfilsertből,  szval lefut a handler.
	
	
    printf("Child process ended\n");  
  }
  return 0;
}