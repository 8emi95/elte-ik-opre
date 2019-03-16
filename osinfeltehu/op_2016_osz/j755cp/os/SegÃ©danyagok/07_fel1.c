//-------------------- 
// SigKuldKap.c 
//-------------------- 
#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>         // Például a fork függvény. 
#include <signal.h>         // Például a kill függvény. 
#include <sys/types.h>      // Például a pid_t típus. 
 
Void kezdjbele(int jelzes) { 
     // A callback függvény, paramétere a szignál számértéke 
     // a gyermekfolyamatnak szóló jelzéshez tartozó függvény. 
     Printf(„Most kaptam egy jelzest – elkezdek dolgozni\n”); 
     int i; 
     for ( i = 0; i < 10; i++) { 
          printf(„%i,”, rand() % 100); 
     } 
     printf(„\n”); 
     kill(getppid(), SIGUSR2); 
     // Küld egy jelzést a szülõfolyamatnak, hogy kész van. 
} 
 
void kesz(int jelzes) { 
     // A callback függvény, paramétere a szignál számértéke 
     // a szülõfolyamatnak szóló jelzéshez tartozó függvény. 
     Printf(„Vegre kesz!\n”); 
} 
 
int main(int argc, char * argv[]) { 
     pid_t child; 
     int status; 
     int i; 
     printf(„Kezdem\n”); 
     child = fork(); // A gyermekfolyamat létrehozása. 
     If (child < 0) { 
          perror(„hiba\n”); 
          exit(1); 
     } 
     if (child == 0) { // A gyermek. 
          Signal(SIGUSR1, kezdjbele); 
          // A gyermekfolyamathoz tartozó jelzéskezelõ beállítása. 
          For( i = 0; i < 10; i++) { 
               printf(„Unatkozom\n”); 
               sleep(1); 
          } 
     } else { 
          signal(SIGUSR2, kesz); 
          // A szülõfolyamathoz tartozó jelzéskezelõ beállítása. 
          Int ido = rand() % 5; 
          printf(„%i s mulva jelzest kuldok a gyermeknek\n”, ido); 
          sleep(ido); 
          kill(child, SIGUSR1); 
          // Jelzést küld a gyermeknek, hogy kezdjen bele a 
          // véletlen szám generálásába. 
          Waitpid(child, &status, 0); 
          printf(„Vege a szulofolyamatnak\n”); 
       }; 
       return 0; 
} 
//-------------------- 
