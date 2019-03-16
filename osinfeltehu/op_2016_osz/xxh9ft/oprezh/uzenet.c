#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

#include <sys/stat.h>
#include <time.h>

void handler(int signumber){
  //printf("Signal with number %i has arrived\n",signumber);
}

struct uzenet {
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

// sendig a message
int kuld( int uzenetsor, pid_t pid, char msg[1024] ) {
     /*const*/ struct uzenet uz;
     uz.mtype = pid;
     strcpy(uz.mtext, msg);
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
     if ( status < 0 ) {
          perror("msgsnd"); 
     }
     return 0;
}
     
// receiving a message. 
int fogad( int uzenetsor, pid_t pid) {
     struct uzenet uz; 
     int status; 
     status = msgrcv(uzenetsor, &uz, 1024, pid, 0);
     
     if ( status < 0 ) {
          perror("msgsnd");
     } else {
          printf( "%s\n", uz.mtext );
          //printf( "%s vagyok, a kapott uzenet kodja: %ld, szovege:  %s\n", getpid(), uz.mtype, uz.mtext );
     }

     return 0;
}

int main (int argc, char* argv[]) {
    printf("\n");
 if( argc == 2 ) {
      //printf("A kapott parameter: %s\n", argv[1]);
   }
   else if( argc > 2 ) {
      printf("Too many arguments supplied.\n");
   }
   else {
      printf("One argument expected.\n");
   }
          signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 

     pid_t child; 
     int uzenetsor, status; 
     int max=atoi(argv[1]);
     key_t kulcs; 
     
     // msgget needs a key, amelyet az ftok general 
     //. 
    //kulcs = ftok(argv[0],1); // Ez nem kell, ha csak a gyereknek küldünk
     //printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); // kulcs helyett lehet IPC_PRIVATE, amikor csak a gyereknek kell küldeni
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
     child = fork();
     // parent
     if ( child > 0 ) {
         // elkuldi, hanyadik szerelveny, es mennyi az osszes szerelvenyszam
        srand(time(NULL));
        char hanyadik[7];
        snprintf(hanyadik, sizeof(hanyadik), "%d", rand()%max+1);
        kuld(uzenetsor, child, hanyadik);
        kuld(uzenetsor, child, argv[1]);


         // jelzest fogad, h a szerelveny beert, vagy bevontattak
        pause();
        printf("Jelzes erkezett.\n",SIGTERM);

         // fogadja, es kiirja, hogy miert jott a jelzes (beert, vagy vontattak)

          //kuld( uzenetsor, child, "Szulo ezt kuldte." );  // Parent sends a message. 
          fogad( uzenetsor, getpid() );
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL);
          wait( NULL ); // megvárja a gyerekfolyamat befejeződését, ide nem feltétlen szükséges
          if ( status < 0 ) {
               perror("msgctl");
          }
          return 0;
     }


     // child
     else if ( 0 == child ) {
         // megkapja hanyadig szerelveny.
        //fogad( uzenetsor, getpid() );
        int hanyadik, osszesen;
        pid_t pid=getpid();
        struct uzenet uz; 
        int status; 
        status = msgrcv(uzenetsor, &uz, 1024, pid, 0);
        
        if ( status < 0 ) {
            perror("msgsnd");
        } else {
            //printf( "%d vagyok, a kapott uzenet kodja: %ld, szovege:  %s\n", getpid(), uz.mtype, uz.mtext );
            hanyadik=atoi(uz.mtext);
            //printf("Ez int: %d\n", hanyadik);
        }

        // mennyi az osszes szerelvenyszamot
        status = msgrcv(uzenetsor, &uz, 1024, pid, 0);
        
        if ( status < 0 ) {
            perror("msgsnd");
        } else {
            //printf( "%d vagyok, a kapott uzenet kodja: %ld, szovege:  %s\n", getpid(), uz.mtype, uz.mtext );
            osszesen=atoi(uz.mtext);
            //printf("Ez int: %d\n", osszesen);
        }
        
         // elcsuszik vagy lefagy
         // kiirja hanyadi, es hany van utana, es h sikeres volt-e.
        int siker=1;
        srand(time(NULL));
        printf("gyerek: Ennyiedik vagyok: %d, ennyi van utanam: %d\n", hanyadik, osszesen-hanyadik);
        if(0==rand()%10) {
            siker=0;
            printf("gyerek: Elcsusztam, vagy lefagytam...\n");
        } else {
            printf("gyerek: Sikeres menet volt.\n");
        }

         // jelzes, hogy beert, vagy bevontattak
        kill(getppid(), SIGTERM);

         // konkret okot, uzenetsoron
         if(siker){
            return kuld( uzenetsor, getppid(), "A gyerek sikeresen beert!\n" );  // child sends a message. 
         } else {
              return kuld( uzenetsor, getppid(), "A gyerek elcsuszott, vagy lefagyott.\n" );  // child sends a message. 
         }
     } else { 
          perror("fork"); 
          return 1; 
     }
     
     return 0; 
} 
