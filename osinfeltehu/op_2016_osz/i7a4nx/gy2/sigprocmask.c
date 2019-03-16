#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

//peldaa annak lehetosegere hogy a jelzest egy idore blokkoljuk
//ignoralasnal elveszik a jelzest
//blokkolasnal nem
//erre valo a sigprocmask, SIG_BLOCK
//sigset_t tipusu halmazban helyezzuk el azokat a jelzeseket amiket blokkolni szeretnenek
//sigemptyset hozza letre uresen
//sigaddset() ad hozza

//a kod maga egy gyerekfolyamt lterehozasabaol allocmem
//a szulo varakozik egy jelzesre
//a gyerek var valameddig majd kuld egy sigtermet amit azonban blokkoltunk, igy nem toretneik semmi, csak a SIGUSR1 re
//ezutan a szulo feloldja a blokkolasat a SIGTERMNEK, ezert kesobb az is megerkezik, mint szo volt rola, nem veszik el

void handler(int signumber){
  //altalban keves kodot es jellemzoen nem printeket rakunk esemynekezelobe
  //a belso kod megszakithatosaga is fontos
  printf("Signal with number %i has arrived\n",signumber);
}

int main(){

  sigset_t sigset;
  sigemptyset(&sigset); //empty signal set
  //hozzaadjuk a jelzeseket amiket blokkolni akarjuk
  sigaddset(&sigset,SIGTERM); //SIGTERM is in set
  //sigfillset(&sigset); //each signal is in the set
  sigprocmask(SIG_BLOCK,&sigset,NULL); //signals in sigset will be blocked
  //parameters, how: SIG_BLOCK, SIG_UNBLOCK, SIG_SETMASK -   ; 
  //2. parameter changes the signalset to this if it is not NULL,
  //3.parameter if it is not NULL, the formerly used set is stored here
    
  signal(SIGTERM,handler); //signal and handler is connetcted
  signal(SIGUSR1,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
  
  pid_t child=fork();
  if (child>0)
  { 
	//a szuloben eloszor egy pause, varunk mig jelzes nem erkezik
    pause(); //waits till a signal arrive SIGTERM is blocked, so it gets SIGUSR1
    printf("The blocking of SIGTERM %i signal is released, so the formerly sent SIGTERM signal arrives\n",SIGTERM);
	//feloldjuk a blokkolast
	//ezaltal a folyamtra varakozo signal mar megerkezhet a folyamathoz
    sigprocmask(SIG_UNBLOCK,&sigset,NULL); 
    //SIGTERM is now released (not blocked further), the process will get it
    
    int status;
    wait(&status);
    printf("Parent process ended\n");
  }
  else 
  {
    printf("Waits 3 seconds, then send a SIGTERM %i signal (it is blocked)\n",SIGTERM);
    sleep(3);
	//kuldunk egy sigterm szignalt a szulonek... persze ez blokkolva van
    kill(getppid(),SIGTERM);
    printf("Waits 3 seconds, then send a SIGUSR1 %i signal (it is not blocked)\n", SIGUSR1);
    sleep(3);
    kill(getppid(),SIGUSR1);
	//sigterm, sigusr1 volt a kuldesi sorrend, de a blokkolas miatt forditott sorrendben erkeznek
    printf("Child process ended\n");  
  }
  return 0;
}