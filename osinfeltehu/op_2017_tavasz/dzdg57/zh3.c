#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>        // Például a fork. 
#include <sys/types.h>     // Például a pid_t típus. 
#include <sys/wait.h>      // Például a wait, és a waitpid.
#include <string.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <wait.h> 


struct uzenet { 
     long mtype; 
     char mtext [ 50 ]; 
}; 

// Üzenetet küld az üzenetsorba. 
int kuld( int uzenetsor, char text[50] ) { 
     const struct uzenet uz = { 8, text }; 
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
     // Küld egy jelzést a szülõfolyamatnak, hogy kész van. 
}

void kesz(int jelzes) { 
     // A callback függvény, paramétere a szignál számértéke 
     // a szülõfolyamatnak szóló jelzéshez tartozó függvény. 
     printf("Az utas jelentkezett!\n"); 
	 
} 

int main(int argc, char * argv[]) { 
	int status; 
    printf("A szulo  %i PID szamu\n", getpid()); 
	pid_t child1, child2; 
	 
	//
	pid_t child; 
    int uzenetsor; 
    key_t kulcs; 
     
    // Létrehozáskor adni kell egy kulcsot, amelyet az ftok 
    // függvénnyel generálhatunk. 
    kulcs = ftok(argv[0],1); 
     
    uzenetsor = msgget( kulcs, 0700 | IPC_CREAT ); 
    if ( uzenetsor < 0 ) { 
        perror("msgget"); 
         return 1; 
    }
	//
	
	int pipe1[2];
    pipe(pipe1);
	 
     child1 = fork(); 
     if (child1==0) { // Gyermekfolyamat.
		
	 	signal(SIGUSR1, kezdjbele); // gyermekfolyamathoz tartozó jelzéskezelõ beállítása

		  
		  char cim[50];
		  strcpy(cim ,argv[1]);
		  strcat(cim, "\n");
		  printf("A lakcím az argumentum alapján: %s, hossz: %i\n", cim, strlen(cim));
		  
		  signal(SIGUSR1, kezdjbele); // gyermekfolyamathoz tartozó jelzéskezelõ beállítása
		  
		  close(pipe1[0]);
		  write(pipe1[1], cim , strlen(cim));
		  close(pipe1[1]);
		  
		  printf("vege az utas folyamatnak \n");
          
     } else { // A szülõfolyamat. 
		sleep(1);
        
		
        child2 = fork();   // A gyermekfolyamat létrehozása. 
        if (child2 == 0) { // A szülõ második gyermeke. 
            sleep(1);
			printf("A szulo 2.gyermeke, azaz a kiszolgáló autó -PID %i\n", getpid() );
			fogad(uzenetsor);
			
        } else { 
			printf("A  szulo  itt kezdodik,-PID %i\n", getpid()); 
			signal(SIGUSR2, kesz); 	// beállítjuk a SIGUSR2 szignál kezelõjének a kesz függvényt
			kill(child1, SIGUSR1); 
			//pause(); 	// a jelzés érkezéséig vár
			
			char s[50];
			close(pipe1[1]);
			read(pipe1[0],s ,sizeof(s));
			printf("szuloben kiolvastuk a pipe tartalmat: %s",s);  
			close(pipe1[0]);
			
			
			waitpid(child1, &status, 0); // Várunk a szülõ elsõ gyermekére. 
            printf("A szulo  (1.gyereket bevarta)-PID %i\n", getpid());
			
			kuld( uzenetsor, s );
			
            waitpid(child2, &status, 0); // Várunk a második gyermekre. 
            printf("A szulo  (2.gyereket bevarta)-PID %i\n", getpid()); 
             
        } 
     } 
     return 0; 
} 