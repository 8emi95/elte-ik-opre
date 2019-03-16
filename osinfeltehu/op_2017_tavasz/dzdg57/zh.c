#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>        // Például a fork. 
#include <sys/types.h>     // Például a pid_t típus. 
#include <sys/wait.h>      // Például a wait, és a waitpid.
#include <string.h>

struct uzenet { 
     long mtype; 
     char mtext [ 1024 ]; 
}; 

// Üzenetet küld az üzenetsorba. 
int kuld( int uzenetsor ) { 
     const struct uzenet uz = { 8, "Hello!" }; 
     int status; 
     
     status = msgsnd( uzenetsor, & uz, strlen ( uz.mtext ) + 1 , 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// Üzenetet fogad az üzenetsorból. 
int fogad( int uzenetsor ) { 
     struct uzenet uz; 
     int status; 
     
     status = msgrcv( uzenetsor, & uz, 1024, 0, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A kapott uzenet: %ld, %s\n", uz.mtype, uz.mtext ); 
     return 0; 
} 


void kezdjbele(int jelzes) { 
     // A callback függvény, paramétere a szignál számértéke 
     // a gyermekfolyamatnak szóló jelzéshez tartozó függvény. 
     printf("megvan a cím\n"); 
     kill(getppid(), SIGUSR2); 
     // Küld egy jelzést a szülőfolyamatnak, hogy kész van. 
}

void kesz(int jelzes) { 
     // A callback függvény, paramétere a szignál számértéke 
     // a szülőfolyamatnak szóló jelzéshez tartozó függvény. 
     printf("Az utas jelentkezett!\n"); 
	 
} 

int main(int argc, char * argv[]) { 
     int status; 
     printf("A szulo  %i PID szamu\n", getpid()); 
     pid_t child1, child2; 
	 

	int pipe1[2];
    pipe(pipe1);
	 
     child1 = fork(); 
     if (child1==0) { // Gyermekfolyamat.
		
	 	signal(SIGUSR1, kezdjbele); // gyermekfolyamathoz tartozó jelzéskezelő beállítása

		  
		  char cim[50];
		  strcpy(cim ,argv[1]);
		  strcat(cim, "\n");
		  printf("A lakcím az argumentum alapján: %s, hossz: %i\n", cim, strlen(cim));
		  
		  signal(SIGUSR1, kezdjbele); // gyermekfolyamathoz tartozó jelzéskezelő beállítása
		  
		  close(pipe1[0]);
		  write(pipe1[1], cim , strlen(cim));
		  close(pipe1[1]);
		  
		  printf("vege az utas folyamatnak \n");
          
     } else { // A szülőfolyamat. 
		sleep(1);
        
		
        child2 = fork();   // A gyermekfolyamat létrehozása. 
        if (child2 == 0) { // A szülő második gyermeke. 
            printf("A szulo 2.gyermeke, azaz a kiszolgáló autó -PID %i\n", getpid() ); 
        } else { 
			printf("A  szulo  itt kezdodik,-PID %i\n", getpid()); 
			signal(SIGUSR2, kesz); 	// beállítjuk a SIGUSR2 szignál kezelőjének a kesz függvényt
			kill(child1, SIGUSR1); 
			//pause(); 	// a jelzés érkezéséig vár
			
			char s[50];
			close(pipe1[1]);
			read(pipe1[0],s ,sizeof(s));
			printf("szuloben kiolvastuk a pipe tartalmat: %s",s);  
			close(pipe1[0]);
			
			waitpid(child1, &status, 0); // Várunk a szülő első gyermekére. 
            printf("A szulo  (1.gyereket bevarta)-PID %i\n", getpid());
			
			
            waitpid(child2, &status, 0); // Várunk a második gyermekre. 
            printf("A szulo  (2.gyereket bevarta)-PID %i\n", getpid()); 
             
        } 
     } 
     return 0; 
} 