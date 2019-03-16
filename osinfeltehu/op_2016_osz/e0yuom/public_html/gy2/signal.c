#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  signal(SIGTERM,SIG_DFL); //azért hívjuk meg, hogy tényleg le is álljon, visszaállítja a signalt alaphelyzetbe
  raise(SIGTERM); //küld a program magának egy signalt ami pont a leállítás
}

int main(){

    
  signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
  // signal(SIGTERM,SIG_IGN); // nem fog egyáltalán reagálni a sigtermre
                           //handler = SIG_DFL - back to default behavior 
						   //ezzel kapom el a sigterm signalt és helyette futtatom a handler függvényt
						   //signalkezelés, fut a szülőfolyamat és a gyerekfolyamat futásán kívül
  
  pid_t child=fork();   //generál egy gyerekfolyamatot
  if (child>0)			//szülőfolyamat
  { 
	//ha itt lenne egy sleep (3+++) akkor nem kapná meg a signalt mivel később kezdené el várni a signalt egyáltalán
    pause(); //waits till a signal arrive várakozik egy jelzésre
    printf("Signal arrived\n",SIGTERM);
    int status;
    wait(&status);
    printf("Parent process ended\n");
  }
  else //gyerekfolyamat
  {
    printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
    sleep(3);	// várakozik 3 másodprecet
    kill(getppid(),SIGTERM); // itt küld a szülőfolyamat pidszámára egy signalt, getppid a szülő folyamatszámát adja meg, kill hasonlóan mint terminálból
    //1. parameter the pid number of process, we send the signal
    // 		if -1, then eacho of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
    printf("Child process ended\n");  
  }
  return 0;
}

// háttérben elindítani a programot: a.out &  // az and jel kell
//kill paranccsal küldök jelzést a terminálban kill -SIGTERM 12345 // a végén a szám a folyamat száma amit a háttérben elindításkor kiír, a sigterm pedig hogy álljon le

//ps paranccsal meg lehet nézni hogy milyen beragadt folyamatok vannak
//kill-el lehet leállítani kill -SIGKILL pidszámotide