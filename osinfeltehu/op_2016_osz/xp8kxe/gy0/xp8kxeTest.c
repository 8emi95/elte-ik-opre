#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>        // P�ld�ul a fork. 
#include <sys/types.h>     // P�ld�ul a pid_t t�pus. 
#include <sys/wait.h>      // A wait �s a waitpid f�ggv�ny haszn�lat�hoz. 
int main() { // Az unistd.h header f�jlt be kell szerkeszteni. 
     int status; // A wait f�ggv�nyben a gyermekfolyamat �llapota. 
     pid_t child = fork(); 
     // Itt keletkezik a gyermekfolyamat, amelyik a sz�l�folyamat 
     // m�solatak�nt foghat� fel. 
     // A child v�ltoz�ban a keletkezett gyermekfolyamat PID sz�ma lesz 
     // (process identification, vagy m�s n�ven a folyamat azonos�t�ja) 
     if (child < 0) { 
          perror("Nem sikerult folyamatot letrehozni\n"); 
          exit(1); 
     } 
          // Kil�p�nk, mert nem siker�lt �j gyermekfolyamatot l�trehozni. 
     if (child > 0) { 
          printf("En a szulofolyamat vagyok, PID szamom %i \n", getpid()); 
          // Ezt a k�dr�szletet csak a sz�l�folyamat hajtja v�gre, hiszen 
          // l�tja a child v�ltoz� �rt�k�t, amit az �ltala megh�vott fork 
          // adott vissza a sz�m�ra. 
          waitpid(child, &status, 0); 
          // V�rakoz�s a gyermekfolyamat befejez�d�s�re. 
          if (WIFEXITED(status)) { 
               printf("Normal befejezodes\n"); 
          } else { 
               printf("Hibas befejezodes\n"); 
          } 
          printf("Sz�l� v�ge\n"); // Mindig ez ker�l utolj�ra ki�r�sra. 
     } 
     if (child == 0) { 
          printf("En a gyermekfolyamat vagyok, PID szamom %i \n", getpid()); 
          // Ezt a k�dr�szletet csak a gyermekfolyamat hajtja v�gre, hiszen        
          // a gyermekfolyamat nem l�tja a child v�ltoz� �rt�k�t. 
     } 
     return 0; 
} 