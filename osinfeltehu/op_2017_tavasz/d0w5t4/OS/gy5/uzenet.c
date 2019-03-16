#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

struct message
{
     long mytpe;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ];
};

// sendig a message
int kuld( int message_queue )
{
     const struct message uz = { 5, "Hajra Fradi!" };
     int status;

     status = msgsnd( message_queue, &uz, strlen ( uz.mtext ) + 1 , 0 );
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 )
          perror("msgsnd");
     return 0;
}

// receiving a message.
int fogad( int message_queue )
{
     struct message uz;
     int status;
     // az utolso parameter(0) az message azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol
     status = msgrcv(message_queue, &uz, 1024, 5, 0 );

     if ( status < 0 )
          perror("msgsnd");
     else
          printf( "A kapott message kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext );
     return 0;
}

int main (int argc, char* argv[]) {
     pid_t child;
     int message_queue, status;
     key_t kulcs;

     // msgget needs a key, amelyet az ftok gener�l
     //.
     kulcs = ftok(argv[0],1);
     printf ("A kulcs: %d\n",kulcs);
     message_queue = msgget( kulcs, 0600 | IPC_CREAT );
     if ( message_queue < 0 ) {
          perror("msgget");
          return 1;
     }

     child = fork();
     if ( child > 0 ) {
          kuld( message_queue );  // Parent sends a message.
          wait( NULL );
          // After terminating child process, the message queue is deleted.
          status = msgctl( message_queue, IPC_RMID, NULL );
          if ( status < 0 )
               perror("msgctl");
          return 0;
     } else if ( child == 0 ) {
          return fogad( message_queue );
          // The child process receives a message.
     } else {
          perror("fork");
          return 1;
     }

     return 0;
}
