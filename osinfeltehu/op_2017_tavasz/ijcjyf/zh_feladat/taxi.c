#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>


void handler(int signalnumber){
 printf("Erkezett egy utas!\n");
 
}



int main(int argc, char *argv[]) {
	
	
   signal(SIGTERM,handler);
   // cso letrehozasa
   int pipeUtastol[2];
	
	
	  if (pipe(pipeUtastol) == -1) 
			{
               perror("Hiba a pipe nyitaskor!");
        		exit(EXIT_FAILURE);
           }
	
	
  int fork1 = fork();
  
  if (fork1 < 0) {
    printf("error\n");
  } else if (fork1 == 0) {
	  
    printf("En vagyok az utas (gyerek)! \n");
    printf("utas: parent: %i\n", getppid());
    printf("utas1: my pid: %i\n", getpid());
	
	sleep(1);
	// utas jelez a 
	kill(getppid(),SIGTERM); 
	
	 close(pipeUtastol[0]);// lezarjuk az olvaso veget a csonek
	  
	   //printf("Begepelt uzenet: %s\n", argv[1]);
	   //printf("\n");

	 write(pipeUtastol[1], argv[1],80);	
	 close(pipeUtastol[1]);// lezarjuk az iro veget a csonek
	
	
	
  } else {
    int fork2 = fork();
    if (fork2 < 0) {
      printf("error\n");
    } else if (fork2 == 0) {
      printf("En vagyok az auto (gyerek)!\n");
      printf("auto parent: %i\n", getppid());
      printf("auto 2: my pid: %i\n", getpid());
    } else {
		
      printf("En vagyok a KOZpont! (szulo)\n");
      printf("The PIDs are:\n");
      printf("kozpont pid: %i\n", getpid());
      printf("utas 1: %i\n", fork1);
      printf("auto 2: %i\n", fork2);
	  
	
	
	   pause();  // varjuk az utas bejelentkezését!
	   printf("jott egy utas");
	   
	   
	  close(pipeUtastol[1]); // lezarjuk az iro veget
	  char utasCime[80];  
	//  
	 read(pipeUtastol[0],utasCime,sizeof(utasCime));
	  printf("kapott cim az utastol %s\n",utasCime);		
	  close(pipeUtastol[0]); // lezarjuk az olvaso veget
	
	   
	//	int status;
	//	wait(&status);
	    printf("a kozpont bezart \n");
    }
  }
  return 0;
}