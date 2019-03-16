#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 
#include <signal.h>
#include <sys/types.h>


#include <stdio.h>

int kezdo =0;
	

void handlerFirst(int signalnumber){
 printf("En vagyok az elso jatekos\n");
	if
  kezdo=(rand()%1); //10-20 közötti szam
			
 //kezdo= 1;
}



int main() {
	srand(time(NULL)); 
  int fork1 = fork();
   
   
   signal(SIGTERM,handlerFirst); 
  
  
  if (fork1 < 0) {
    printf("error\n");
  } else if (fork1 == 0) {
	  pause();
	
    printf("I'm child 1\n");
    printf("child 1: parent: %i\n", getppid());
    printf("child 1: my pid: %i\n", getpid());
	
	
	if (kezdo ==1){
		
		 printf("child 1:enkezdek  \n");
		
	}else{
		 printf("child 1: nem en kezdek :( \n"); 
		
	}
	
	 printf("jatszom \n");
	
  } else {
    int fork2 = fork();
    if (fork2 < 0) {
      printf("error\n");
    } else if (fork2 == 0) {
		 pause();
      printf("I'm child 2\n");
      printf("child 2: parent: %i\n", getppid());
      printf("child 2: my pid: %i\n", getpid());
	 
		
	  if (kezdo ==0){
		
		 printf(" child 2: enkezdek  \n ");
		
	}else{
		 printf("child 2 : nem en kezdek :(   \n"); 
		
	}
	
	 printf("child 2: jatszom\n");
	  
    } else {
      printf("I'm the parent\n");
      printf("The PIDs are:\n");
      printf("parent: %i\n", getpid());
      printf("child 1: %i\n", fork1);
      printf("child 2: %i\n", fork2);
	  
	sleep(3);
	   //kill(0,SIGTERM); 
	    kill(fork1,SIGTERM); 
        kill(fork2,SIGTERM); 
		
	   int statusz;
	   wait(&statusz) ;
    }
  }
  return 0;
}