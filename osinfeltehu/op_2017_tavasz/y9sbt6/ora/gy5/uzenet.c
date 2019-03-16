#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

// SYSTEM V-ös példa

struct uzenet { 
     long mtype; //ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext[1024]; // üzenet tartalma: itt egy tömb, de lehet struktúra vagy bármi egyéb is 
}; 

// üzenet küldése
int kuld(int uzenetsor) 
{ 
     const struct uzenet uz = { 5, "Hajra Fradi!" }; 
     int status; 
     
     status = msgsnd(uzenetsor, &uz, strlen(uz.mtext) + 1 , 0); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext): ez az üzenet tartalmának mérete
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if (status < 0)
     { 
          perror("msgsnd");
     }
     
     return 0;
} 
     
// üzenet fogadása 
int fogad(int uzenetsor) 
{ 
     struct uzenet uz; 
     int status;
      
        // az utolso parameter(0) a fogadásra vonatkozó opciókat határozza meg
	// ha a 4. paraméter 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0); 
     
     if (status < 0)
     { 
          perror("msgsnd"); 
     }
     else
     {
          printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext); 
     }
     return 0; 
} 

int main (int argc, char* argv[])
{ 
     pid_t child; 
     int uzenetsor, status; 
     key_t kulcs; 
     
     // msgget needs a key, amelyet az ftok generál 
     //. 
     kulcs = ftok(argv[0], 1); 
     printf("A kulcs: %d\n", kulcs);
     uzenetsor = msgget(kulcs, 0600 | IPC_CREAT); 
     
     if (uzenetsor < 0)
     { 
          perror("msgget"); 
          return 1; 
     } 
     
     child = fork(); 
     if (child > 0) // szülő folyamat
     { 
          kuld(uzenetsor);  // szülő üzenetet küld 
          wait(NULL);
          
          // a gyerekfolyamat terminálását követően töröljük az üzenetsort 
          status = msgctl(uzenetsor, IPC_RMID, NULL); 
          if (status < 0)
          { 
               perror("msgctl"); 
          }
          
          return 0; 
     }
     else if (child == 0) // gyerek folyamat
     {
          return fogad(uzenetsor); // a gyerek fogad egy üzenetet
     }
     else
     { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
