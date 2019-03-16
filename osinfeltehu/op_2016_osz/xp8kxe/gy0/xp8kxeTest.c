#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>        // Például a fork. 
#include <sys/types.h>     // Például a pid_t típus. 
#include <sys/wait.h>      // A wait és a waitpid függvény használatához. 
int main() { // Az unistd.h header fájlt be kell szerkeszteni. 
     int status; // A wait függvényben a gyermekfolyamat állapota. 
     pid_t child = fork(); 
     // Itt keletkezik a gyermekfolyamat, amelyik a szülõfolyamat 
     // másolataként fogható fel. 
     // A child változóban a keletkezett gyermekfolyamat PID száma lesz 
     // (process identification, vagy más néven a folyamat azonosítója) 
     if (child < 0) { 
          perror("Nem sikerult folyamatot letrehozni\n"); 
          exit(1); 
     } 
          // Kilépünk, mert nem sikerült új gyermekfolyamatot létrehozni. 
     if (child > 0) { 
          printf("En a szulofolyamat vagyok, PID szamom %i \n", getpid()); 
          // Ezt a kódrészletet csak a szülõfolyamat hajtja végre, hiszen 
          // látja a child változó értékét, amit az általa meghívott fork 
          // adott vissza a számára. 
          waitpid(child, &status, 0); 
          // Várakozás a gyermekfolyamat befejezõdésére. 
          if (WIFEXITED(status)) { 
               printf("Normal befejezodes\n"); 
          } else { 
               printf("Hibas befejezodes\n"); 
          } 
          printf("Szülõ vége\n"); // Mindig ez kerül utoljára kiírásra. 
     } 
     if (child == 0) { 
          printf("En a gyermekfolyamat vagyok, PID szamom %i \n", getpid()); 
          // Ezt a kódrészletet csak a gyermekfolyamat hajtja végre, hiszen        
          // a gyermekfolyamat nem látja a child változó értékét. 
     } 
     return 0; 
} 