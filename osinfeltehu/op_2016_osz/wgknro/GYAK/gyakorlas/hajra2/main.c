#include <time.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

struct uzenet { 
     int mtype; 
     int mtext; 
}; 

struct uzenet2 {  
     char mtext [ 1024 ]; 
}; 

// �zenetet k�ld az �zenetsorba. 
int kuld( int uzenetsor, int elso, int masodik ) { 
     const struct uzenet uz = { elso, masodik}; 
     int status; 
     
      msgsnd( uzenetsor, & uz, sizeof ( uz ) + 1 , 0 ); 
    /* 
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
	 */
} 

int kuld2( int uzenetsor, char* szoveg ) { 
     struct uzenet2 uz; 
		strcpy (uz.mtext, szoveg);
     
      msgsnd( uzenetsor, & uz, sizeof ( uz ) + 1 , 0 );     
}
     
// �zenetet fogad az �zenetsorb�l. 
int fogad( int uzenetsor ) { 
     struct uzenet uz; 
     int status; 
     
     status = msgrcv( uzenetsor, & uz, 1024, 0, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "[GYEREK] A kapott uzenet: %d, %d\n", uz.mtype, uz.mtext ); 
	  printf( "[GYEREK] %d. szerelveny vagyok \n", uz.mtype);
	  int hatravan = (uz.mtype - uz.mtext) * -1;
	  printf( "[GYEREK] Meg hatra van %d szerelveny \n",hatravan);
		
     return 0; 
} 


// �zenetet fogad az �zenetsorb�l. 
int fogad2( int uzenetsor ) { 
     struct uzenet2 uz; 
     
     msgrcv( uzenetsor, & uz, 1024, 0, 0 ); 
    
		printf( "[SZULO] A kapott uzenet a gyerektol: %s \n", uz.mtext ); 
		
     return 0; 
}

int main (int argc, char *argv[]) { 
	int max = atoi(argv[1]); 
     pid_t child; 
     int uzenetsor, status; 
     key_t kulcs; 
     
     // L�trehoz�skor adni kell egy kulcsot, amelyet az ftok 
     // f�ggv�nnyel gener�lhatunk. 
     kulcs = ftok(argv[0],1); 
     
     uzenetsor = msgget( kulcs, 0700 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
     child = fork(); 
     if ( child > 0 ) {  // sz�l�
			int val = (rand() % (max - 1) )  + 1;
          kuld( uzenetsor, val, max );  // A sz�l�folyamat k�ldi az �zenetet. 
		  fogad( uzenetsor ); 
          //wait( NULL ); 
          // Miut�n a gyerek v�get �rt, t�rli az �zenetsort. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { 
          fogad( uzenetsor ); 
		  char *child_msg = "Minden OKE \n";
		  kuld2( uzenetsor, child_msg);  // A gyerek k�ldi az �zenetet. 
		  return 0;
          
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
}