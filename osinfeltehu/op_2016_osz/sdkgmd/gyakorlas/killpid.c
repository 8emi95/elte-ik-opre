//-------------------- 
// SigKuld.c 
//-------------------- 
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <signal.h>         // Például a kill függvény. 
#include <sys/types.h>      // Például a pid_t típus. 
int main(int argc, char * argv[]) { 
     int PID; 
     if (argc == 1) { 
          printf("Parameter kellene\n"); 
          exit(1); 
     } 
     PID = atoi(argv[1]); 
     kill(PID, SIGSTOP); // Leállítja a folyamatokat. 
     printf("Stop\n"); 
     fflush(stdout); 
     system("/bin/ps u"); 
     sleep(5); 
     kill(PID,SIGCONT); // Folytatja a leállítottat. 
     printf("Folytat\n"); 
     fflush(stdout); 
     system("/bin/ps u"); 
     sleep(5); 
     kill(PID, SIGKILL); // Megszakítja a futását. 
     printf("Befejezodott\n"); 
     fflush(stdout); 
     system("/bin/ps u"); 
      return 0; 
} 
//--------------------
