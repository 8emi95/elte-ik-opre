#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>

struct uzenet {

   char cim[200];
   pid_t telefon;

};


void handler() {



}


int main(int argc, char *argv[]) {

   //int status;
   int pipefd[2];

   if ( pipe ( pipefd ) == -1 ) { perror("Hiba a pipe letrehozasakor\n"); exit(1); }

   pid_t utas = fork();

   if ( utas == -1 ) { perror("hiba a gyerekfolyamat letrehozasakor\n"); exit(1); }

   // Szignál
   struct sigaction sigact;
   sigact.sa_handler = handler;
   sigemptyset(&sigact.sa_mask);
   
   sigact.sa_flags = 0;
   sigaction(SIGUSR1, &sigact, NULL);

   if(utas > 0) {

     printf("A kozpont varja az utasokat...\n");
     char cim[200];
	
     int status;
  
     sigset_t sigset;
     sigfillset(&sigset);
     sigdelset(&sigset, SIGUSR1);
     sigsuspend(&sigset);

     printf("Utas erkezett\n");
     
     close(pipefd[1]);
     read(pipefd[0], cim, sizeof cim);
     close(pipefd[0]);

     int pipefd2[2];
     if( pipe ( pipefd2 ) == -1) { perror("Hiba a csovezetek2 letrehozasakor\n"); exit(1); }

     struct uzenet msg;
     strcpy(msg.cim, cim);
     msg.telefon = utas;


     pid_t taxis = fork();

     if( taxis == -1 ) { perror("Hiba a taxis letrehozasakor\n"); exit(1); }

     if( taxis > 0 ) { // Központ

	close(pipefd2[0]);
        write(pipefd2[1], &msg, sizeof msg);
        close(pipefd2[1]);

     } else {

        struct uzenet bej;
	close(pipefd2[1]);
        read(pipefd2[0], &bej, sizeof bej);
        close(pipefd2[0]);

     printf("Utas telefonszama: %d\n", bej.telefon);
     printf("Utas cime: %s\n", bej.cim);
        printf("Taxis: megkapta cimet, kikuldte a taxit\n", bej.cim, bej.telefon);
        printf("Taxis befejezte\n");

     }

     waitpid(taxis, &status, 0);

     printf("Kozpont befejezte\n");

   }
 
   else {
      
      sleep(3);
      printf("Az utas adja meg a cimet: \n");

      write(pipefd[1], argv[1], sizeof(argv[1]));
      close(pipefd[1]);
      fflush(NULL);      
      kill(getppid(), SIGUSR1);

      printf("Utas vege\n");
   }

   return 0;

}
