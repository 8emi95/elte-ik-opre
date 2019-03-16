#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>

#include "question.h"
#include "printall.h"
#include "add.h"
#include "edit.h"
#include "kerdobiztos.h"

int pipefd[2];
pid_t pid;
struct question questions[32]; // Maximálisan 32 db kérdést lehet
int k = 0; // A kérdések száma

void printmenu(){
  
  int i = 0;
  for(i = 0; i < 5; i++) { printf("\n"); }
  printf("-------------------------------------------\n");
  printf("              MLSZ Kérdőív                 \n");
  printf("-------------------------------------------\n");
  printf("\n");
  printf("Válasszon egy lehetőséget! Írja be a választott opciónak a számát!\n");
  printf("1 - Új kérdés felvétele\n");
  printf("2 - Kérdések módosítása\n");
  printf("3 - Kérdések törlése\n");
  printf("4 - Kérdőív véglegesítése\n");
  printf("5 - Végleges kérdőív megtekintése\n");
  printf("Választott menü: ");
  fflush(stdout);
  
}

void copyfiles() {
      FILE *src, *dst;
      char c;

      src = fopen("kerdesek.txt", "r");
      dst = fopen("vegleges.txt", "w");
      
      do {
	c = fgetc(src);
        if(c == EOF) break;
        fputc(c, dst);
      } while (c != EOF);

      fclose(src);
      fclose(dst);
}

void printinput(){

  int input = 1;
  char* in = malloc(8*sizeof(char));
  
  while(input){
    fgets(in, sizeof(in), stdin);
    int i_in = atoi(in); 
    if(i_in >= 0 && i_in < 6){
      input = 0;
    }else{
      printf("Hibás bemenet! Próbálja újra: ");
    }
  }
  
  switch(atoi(in)){
    case 1 :
      // Új kérdés
      addmenu();
      printmenu();
      printinput();
      break;
    case 2 :
      // Módosítás
      printall("kerdesek.txt");
      edit();
      printmenu();
      printinput();
      break;
    case 3 :
      // Törlés
      printall("kerdesek.txt");
      deleteq();
      printmenu();
      printinput();
      break;
    case 4 :
      // Véglegesítés: Másolás egy új fájlba (vegleges.txt)
      copyfiles();

	  // Kérdőbiztos kiküldése
	  readdata(questions, &k); // Adatok beolvasása a question struktúrába

	  // Írás a csővezetékre
	  // Szignál kiküldése a kérdőbiztosnak, kezdheti a munkát
	  struct question tosend[3];
	  int i = 0;
	  for (; i < 3; i++) {
		  tosend[i] = questions[rand() % k];
	  }

	  write(pipefd[1], tosend, sizeof tosend);
	  close(pipefd[1]);

	  fflush(NULL);

	  kill(pid, SIGUSR1);

	  int status;
	  wait(&status);
	  printf("MLSZ Befejezte a munkat.\n");

	  printmenu();
	  printinput();
	  break;
    case 5 :
      printall("vegleges.txt");
      printmenu();
      printinput();
      break;
    case 0 :
      break;
  }

  free(in);
}

void handler() {

}

int main(int argc, char** args){

	if (pipe(pipefd) == -1) {
		printf("Hiba a pipe letrehozasakor.\n");
		return 1;
	}

	pid = fork();

	struct question pipe_buf[3];

	struct sigaction sigact;
	sigact.sa_handler = handler; //SIG_DFL,SIG_IGN
	sigemptyset(&sigact.sa_mask); //during execution of handler these signals will be blocked plus the signal    
								  //now only the arriving signal, SIGTERM will be blocked
	sigact.sa_flags = 0; //nothing special behaviour
	sigaction(SIGUSR1, &sigact, NULL);

	if (pid < 0) return 1;
	if (pid > 0) { // Szülő

		// Menü kiírása
		printmenu();

		// Fájl létrehozása (ha nem létezik)
		int f;
		if (access("kerdesek.txt", F_OK) == -1) {
			f = open("kerdesek.txt", O_CREAT, 0777);
			close(f);
		}

		// Input
		printinput();

	}
	else { // Gyerek
		// Várakozunk a SIGUSR1 szignálra
		sigset_t sigset;
		sigfillset(&sigset);
		sigdelset(&sigset, SIGUSR1);
		sigsuspend(&sigset);

		printf("Kerdobiztos: Megerkezett a szignal\n");
		
		close(pipefd[1]);
		read(pipefd[0], pipe_buf, sizeof pipe_buf);
		close(pipefd[0]);

		
		srand(time(NULL));
		int kitoltok = (rand() % 11) + 10;
		int valaszok[3 * kitoltok];
		int n;
		int m;
		int l = 0;
		for (n = 0; n < kitoltok; n++) {
			for (m = 0; m < 3; m++) {
				valaszok[l] = rand() % pipe_buf[m].qnum;
				l = l + 1;
			}
		}
		int j = 0;
		for (; j < sizeof(valaszok) / sizeof(int); j++) {
			int hash = sizeof(valaszok) / sizeof(int) / 3;
			if (j < hash)
				printf("1. kerdesre %d. valasz: %d\n", j + 1, valaszok[j] + 1);
			if (j < hash * 2 && j > hash)
				printf("2. kerdesre %d. valasz: %d\n", j + 1, valaszok[j] + 1);
			if (j < hash * 3 && j > hash * 2)
				printf("3. kerdesre %d. valasz: %d\n", j + 1, valaszok[j] + 1);
		}
		
		printf("Kerdobiztos befejezte a munkat.\n");
	}

  return 0;
}