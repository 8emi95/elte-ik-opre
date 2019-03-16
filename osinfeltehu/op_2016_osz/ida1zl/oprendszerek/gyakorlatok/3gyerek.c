
//-------------------- 
// Forkok2.c 
//-------------------- 
#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>        // Például a fork. 
#include <sys/types.h>     // Például a pid_t típus. 
#include <sys/wait.h>      // Például a wait, és a waitpid. 
int main() { 
     int status; 
     printf("A szulo  %i PID szamu\n", getpid()); 
     pid_t child1, child2; 
     child1 = fork(); 
     if (child1==0) { // Gyermekfolyamat. 
          child2 = fork(); 
          if (child2 == 0){ // Az első gyermek első gyermeke. 
               printf("1.gyermek 1.gyermeke,-PID %i\n", getpid() ); 
          } else { 
               printf("1.gyermek szulo aga,-PID %i\n", getpid()); 
               waitpid(child2, &status, 0); 
               // Várunk az első gyermek első gyermekére. 
               printf("Az elso gyerek bevarta a gyereket\n "); 
          } 
     } else { // A szülőfolyamat. 
          printf("A  szulo  itt kezdodik,-PID %i\n", getpid()); 
          child2 = fork();   // A gyermekfolyamat létrehozása. 
          if (child2 == 0) { // A szülő második gyermeke. 
               printf("A szulo 2.gyermeke, -PID %i\n", getpid() ); 
          } else { 
               waitpid(child2, &status, 0); // Várunk a második gyermekre. 
               printf("A szulo  (2.gyereket bevarta)-PID %i\n", getpid()); 
               waitpid(child1, &status, 0); // Várunk a szülő első gyermekére. 
               printf("A szulo  (1.gyereket bevarta)-PID %i\n", getpid()); 
          } 
     } 
     return 0; 
} 
//--------------------

int pid1 = fork();
if (pid1 > 0) //a szuloben vagyunk
{
	int pid2 = fork();
}