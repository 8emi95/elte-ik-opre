#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
#include <sys/time.h>

struct uzenet {
   long mtype;
   pid_t telefonszam;
};


struct ajanlat {
   char nev[1024];
   int ar;
};

void handler() {

}

int kuld ( int uzenetsor, pid_t telefon ) {
   const struct uzenet uz = { 1, telefon };
   int status;

   status = msgsnd( uzenetsor, &uz, sizeof(uz), 0 );
   if( status < 0 )
	perror("msgsnd");
   return 0;
}

int fogad ( int uzenetsor, struct uzenet uz ) {
   int status;
   status = msgrcv( uzenetsor, &uz, sizeof(uz), 1, 0);
   if( status < 0 )
	perror("msgrcv");
   return 0;
}

int main(int argc, char *argv[]){

   int uzenetsor, status;
   key_t kulcs;
   kulcs = ftok(argv[0],1);

   int pipefd[2];
   if ( pipe ( pipefd ) == -1 ) { perror("Hiba: pipe"); exit(1); }

   uzenetsor = msgget( kulcs, 0600 | IPC_CREAT );
   if ( uzenetsor < 0 ) { perror("msgget"); exit(1); }

   pid_t ugynok = fork();
   pid_t vasarlo = fork();
   if ( vasarlo == -1 ) { perror("Hiba: fork"); exit(1); }
   if ( ugynok == -1 ) { perror("Hiba: fork"); exit(1); }

   struct sigaction sigact;
   sigact.sa_handler = handler;
   sigemptyset(&sigact.sa_mask);

   sigact.sa_flags = 0;
   sigaction(SIGUSR1, &sigact, NULL);

   if( ugynok > 0 && vasarlo > 0) { // Szülő
 
    	kuld( uzenetsor, vasarlo );

   }
   if( ugynok == 0 && vasarlo != 0) { // Ügynök
	   
      struct uzenet uz;
      struct ajanlat al = { *argv[1], atoi(argv[2]) };

      fogad( uzenetsor, uz );

      printf("(Ügynök) Megkaptam az ügyfél telefonszámát \n");

      write(pipefd[1], &al, sizeof(struct ajanlat));
      close(pipefd[1]);

      kill(vasarlo, SIGUSR1);
	
   }
   
   if(vasarlo == 0) {

			 sigset_t sigset;
			 sigfillset(&sigset);
			 sigdelset(&sigset, SIGUSR1);
			 sigsuspend(&sigset);

			 printf("(Vásárló) Hívást kaptam\n");

		  struct ajanlat al;
		  read(pipefd[0], &al, sizeof(struct ajanlat));
		  close(pipefd[0]);

		  printf("(Vásárló) Megkaptam az ajánlatot\n");
		  srand(time(NULL));
		  int elfogadas = (rand() % 2);
		  if(elfogadas) printf("(Vásárló) Elfogadtam az ajánlatot\n");
			  else printf("(Vásárló) Elutasítottam az ajánlatot\n");
		  
	
	}
	
	return 0;

}
