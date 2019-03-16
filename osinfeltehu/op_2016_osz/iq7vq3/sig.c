#include <signal.h> 
#include <stdio.h> 
#include <sys/wait.h> 

#define gyerek 4 
volatile int kids = gyerek; 
static void handler(int sig) { 
     int pid, status; 
     
     printf("A szulo (%d) handler-ben vagyunk, ", getpid()); 
     pid = wait(&status); 
     kids--; 
     printf("az elhalt gyerek id: %d,es meg %d maradt!\n", pid, kids); 
     /* 
     * Újrainstalláljuk a handlert, hogy a SIGCLD szignálra újra 
     * aktivizálódni tudjon, ha van még befejeződött gyerekprocessz. 
     */ 
     sigset(SIGCLD, handler); 
} 
void main() { 
     int i, pid; 
     sigset(SIGCLD, handler); 
     sighold(SIGCLD); 
     for (i = 0; i < gyerek; i++) { 
          if (fork() == 0) { // A gyerekfolyamatban vagyunk. 
               printf("Gyerek processz! Azonositoja: %d\n", getpid()); 
               exit(0); 
          } // Az elágazás vége. 
     } // A ciklus vége. 
     getchar(); 
     while (kids > 0) { 
          printf("Beleptunk a ciklusba!\n"); 
          sigpause(SIGCLD); 
          printf("Most van vege a pausenak!\n"); 
          sighold(SIGCLD); 
     } 
}
