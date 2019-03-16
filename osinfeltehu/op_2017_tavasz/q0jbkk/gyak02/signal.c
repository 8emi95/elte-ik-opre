#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  signal(SIGTERM, SIG_DFL);
  raise(signumber); //aktuális folyamat saját magának küld sigterm jelzést
  //itt nem célszerű printet használni, csak elemi utasításokat (élesben beadandó stb zh mellőzzük)
  int x = 1;
}

int main(){

    
  signal(SIGTERM, handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
  
  pid_t child=fork();
  if (child>0)
  {
    //sleep(3); // ettől itt végtelen ciklus lenne, mert: 3 másodpercig vár a szülő, közben a gyerekfolyamat fut
    //jelzést küld a szülőfolyamatnak, azt meg is kapja a szülő, le is futtatja, de a pause() vár még egy olyan jelzésre
    //ami már lefutott. !!!(tudni kell mi fut le előbb stb) megoldás az int x változóval feljebb!! 
    while(x==0){usleep(10); }//ez is az mint a sleep, csak itt ezredmásodpercben mér
    pause(); //waits till a signal arrive 
    printf("Signal arrived\n",SIGTERM);
    int status;
    wait(&status); //ez az adott folyamat gyerekfolyamatának befejeződésére vár
    printf("Parent process ended\n");
  }
  else 
  {
    printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
    sleep(3); // ez sem szerencsés élő programban, mert ezek párhuzamos folyamatok,
    // de sleep miatt: először a szülőfolymat fut le, ez csak itt arra jó, hogy könnyebben követhető
    kill(getppid(),SIGTERM); //1. param: folyamat száma, 2. param: jelzés 
    //1. parameter the pid number of process, we send the signal
    // 		if -1, then eacho of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
    printf("Child process ended\n");  
  }
  return 0;
}