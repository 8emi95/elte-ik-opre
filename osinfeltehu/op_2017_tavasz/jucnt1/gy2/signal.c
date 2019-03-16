#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
//int x=0;
void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  //Cél: a folyamat hajtsa végre a működést, majd álljon le
  //signal(SIGTERM,SIG_DFL) a követekző sigterm jelzéskor álljon le
  // az lenne célszerű, ha a folyamat magának küldené
  //raise(SIGTERM) függvénnyel leeht megoldani
  //++x
  //handleren belül nem szép printeket használni
  //lista van arról, hogy miket ajánlott használni a handleren belül
}

int main(){

    
  signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior
						   //SIG_USR1 ,2 oprendszer nem használja, ha megkapja a folyamat akkor megáll
  //felülírja a default működést, ahelyett, hogy leállna futtassa le a handlert
  pid_t child=fork();
  if (child>0)
  { 
    pause(); //waits till a signal arrive
	/*
	while(x==0|usleep(5)
		használhatjuk a pause helyett ld. megoldás
	*/
	//pause jelzésig vár
	//wait egy folyamat befejeződésére vár
	//jelzés kill paranccsal
	//ps parancs folyamatok listázása
    printf("Signal arrived\n",SIGTERM);
	kill(child,SIGTERM);
    int status;
    wait(&status);
    printf("Parent process ended\n");
  }
  else 
  {
    printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
    sleep(3);
	//ha a szülőbe tesszük, akkor itt a gyerek folyamat hamarabb küldi el a jelzést, mielőtt a sleep letellik
	//a szülő tehát várni fog egy jelzésre amit már elküldtünk
	//megoldás szinkronizáció
	//cseréljük le a pauset
    kill(getppid(),SIGTERM); 
    //1. parameter the pid number of process, we send the signal
    // 		if -1, then eacho of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
	pause();
    printf("Child process ended\n");  
  }
  return 0;
}