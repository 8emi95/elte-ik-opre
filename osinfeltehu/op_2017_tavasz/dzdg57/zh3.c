#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>        // P�ld�ul a fork. 
#include <sys/types.h>     // P�ld�ul a pid_t t�pus. 
#include <sys/wait.h>      // P�ld�ul a wait, �s a waitpid.
#include <string.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <wait.h> 


struct uzenet { 
     long mtype; 
     char mtext [ 50 ]; 
}; 

// �zenetet k�ld az �zenetsorba. 
int kuld( int uzenetsor, char text[50] ) { 
     const struct uzenet uz = { 8, text }; 
     int status; 
     
     status = msgsnd( uzenetsor, & uz, strlen ( uz.mtext ) + 1 , 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// �zenetet fogad az �zenetsorb�l. 
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
     // A callback f�ggv�ny, param�tere a szign�l sz�m�rt�ke 
     // a gyermekfolyamatnak sz�l� jelz�shez tartoz� f�ggv�ny. 
     printf("megvan a c�m\n"); 
     kill(getppid(), SIGUSR2); 
     // K�ld egy jelz�st a sz�l�folyamatnak, hogy k�sz van. 
}

void kesz(int jelzes) { 
     // A callback f�ggv�ny, param�tere a szign�l sz�m�rt�ke 
     // a sz�l�folyamatnak sz�l� jelz�shez tartoz� f�ggv�ny. 
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
     
    // L�trehoz�skor adni kell egy kulcsot, amelyet az ftok 
    // f�ggv�nnyel gener�lhatunk. 
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
		
	 	signal(SIGUSR1, kezdjbele); // gyermekfolyamathoz tartoz� jelz�skezel� be�ll�t�sa

		  
		  char cim[50];
		  strcpy(cim ,argv[1]);
		  strcat(cim, "\n");
		  printf("A lakc�m az argumentum alapj�n: %s, hossz: %i\n", cim, strlen(cim));
		  
		  signal(SIGUSR1, kezdjbele); // gyermekfolyamathoz tartoz� jelz�skezel� be�ll�t�sa
		  
		  close(pipe1[0]);
		  write(pipe1[1], cim , strlen(cim));
		  close(pipe1[1]);
		  
		  printf("vege az utas folyamatnak \n");
          
     } else { // A sz�l�folyamat. 
		sleep(1);
        
		
        child2 = fork();   // A gyermekfolyamat l�trehoz�sa. 
        if (child2 == 0) { // A sz�l� m�sodik gyermeke. 
            sleep(1);
			printf("A szulo 2.gyermeke, azaz a kiszolg�l� aut� -PID %i\n", getpid() );
			fogad(uzenetsor);
			
        } else { 
			printf("A  szulo  itt kezdodik,-PID %i\n", getpid()); 
			signal(SIGUSR2, kesz); 	// be�ll�tjuk a SIGUSR2 szign�l kezel�j�nek a kesz f�ggv�nyt
			kill(child1, SIGUSR1); 
			//pause(); 	// a jelz�s �rkez�s�ig v�r
			
			char s[50];
			close(pipe1[1]);
			read(pipe1[0],s ,sizeof(s));
			printf("szuloben kiolvastuk a pipe tartalmat: %s",s);  
			close(pipe1[0]);
			
			
			waitpid(child1, &status, 0); // V�runk a sz�l� els� gyermek�re. 
            printf("A szulo  (1.gyereket bevarta)-PID %i\n", getpid());
			
			kuld( uzenetsor, s );
			
            waitpid(child2, &status, 0); // V�runk a m�sodik gyermekre. 
            printf("A szulo  (2.gyereket bevarta)-PID %i\n", getpid()); 
             
        } 
     } 
     return 0; 
} 