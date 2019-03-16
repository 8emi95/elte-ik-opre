#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>       // Például a fork függvény. 
#include <sys/types.h>    // Például a pid_t típus. 
#include <signal.h>       // Például a signal függvény. 
#include <sys/wait.h>     // Például a wait és a waitpid függvény.

void Kezelo2(int sig){
    printf("Kezelo2 \n");
    fflush(stdout);
}

void Kezelo(int sig) {  // A jelzéskezelő függvény fejsora kötött. 
     signal(SIGTERM, SIG_IGN); 
     // Ha jön egy következő SIGTERM, ignoráljuk. 
     printf("%i -s szamu jelzest kaptam \n", sig); 
     sleep(10); 
     // A jelzéskezelő függvény végrehajtása hosszú tíz másodperc. 
     printf("Jelzes vege\n"); 
     fflush(stdout); 
     //signal(SIGTERM, SIG_DFL); 
     raise(SIGTERM);
     // Visszaalítjuk az alapértelmezett kezelési módot, 
     // azaz a leállást. 
} 

int main(int argc, char * argv[]) { 
     int status; 
     int i; 
     signal(SIGTERM, Kezelo);
     signal(SIGUSR1, Kezelo2);
     // Beállítja a SIGTERM szignál kezelőjének a Kezelo függvényt. 
     pid_t child = fork(); 
     if (child > 0) { 
          for ( i = 1; i <= 5; i++ ) { 
               // Eközben érkezik majd a jelzés. 
               sleep(1); 
               printf("Szulo vagyok- mukodok\n"); 
               fflush(stdout); 
          } 
          waitpid(child, &status, 0); 
     } else { 
          sleep(2); 
          // Kis várakozás után küld a szülőnek egy jelzést. 
          kill(getppid(), SIGTERM); 
          // Egy SIGTERM jelzést küld. 
          //sleep(1); 
          //kill(getppid(),SIGTERM); 
          // Újra jelzést küld – az előző biztosan nem fejeződött be. 
     } 
     return 0; 
} 