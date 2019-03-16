#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>        // Például a fork. 
#include <sys/types.h>     // Például a pid_t típus. 
#include <sys/wait.h> 
#include <signal.h>
#include <sys/types.h>    

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  
  
  
}


int main() { 

     signal(SIGTERM,handler); 

     int status; 
     printf("A szulo  %i PID szamu\n", getpid()); 
     pid_t child1, child2; 
     child1 = fork(); 
	 
     if (child1==0) { // Gyermekfolyamat.    	 
         pause();
		 printf("1. kolyok\n");
		 signal(SIGTERM,handler);
		 kill(getppid(),SIGTERM);
         printf("A 1. kolyok vege\n");	
		 
          } 
       else { // A szülőfolyamat. 
          printf("A  szulo  itt kezdodik\n"); 
            // A gyermekfolyamat létrehozása.
            child2 = fork(); 			
            if (child2 == 0) { // A szülő második gyermeke.
               printf("A 2. kolyok\n");	  			
               kill(child2,SIGTERM);
               printf("A 2. kolyok vege\n");			   
          } else { 
               
			   pause();
			    
               printf("Megjott a jel az elso gyerektol\n");
			   waitpid(child2, &status, 0); // Várunk a második gyermekre.
			   printf("Szulo vege\n");
               			   
          } 
     } 
     return 0; 
} 