#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

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

int main () { 
     pid_t child; 
     int uzenetsor, status; 
     key_t kulcs; 
     
     // Létrehozáskor adni kell egy kulcsot, amelyet az ftok 
     // függvénnyel generálhatunk. 
     kulcs = ftok(argv[0],1); 
     
     uzenetsor = msgget( kulcs, 0700 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
     child = fork(); 
     if ( child > 0 ) { 
          kuld( uzenetsor );  // A szülőfolyamat küldi az üzenetet. 
          wait( NULL ); 
          // Miután a gyerek véget ért, törli az üzenetsort. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { 
          return fogad( uzenetsor ); 
          // A gyerekfolyamat fogadja az üzenetet. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
}