#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

// sendig a message
     
// receiving a message. 
void handler(int signumber)
{
    //printf("Signal with number %i has arrived\n", signumber);
}

int main (int argc, char* argv[]) { 
     int uzenetsor, status,uz2; 
     key_t kulcs; 
     signal(SIGTERM,handler);
     // msgget needs a key, amelyet az ftok generál 
     //. 
     kulcs = ftok(argv[0],1); 
     key_t kulcs2;
	     int pipefd[2];

    if (pipe(pipefd) == -1) 
	{
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }

    /* create child */
    pid_t child = fork();
    if (child < 0) 
    {
        perror ("fork() error!");
        exit(-1);
    }

     kulcs2=ftok(argv[0],1);
     printf ("A kulcs: %d\n",kulcs);

     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT );
     uz2 = msgget( kulcs2, 0600 | IPC_CREAT ); 
     //printf("%i\n",uz2);
     //printf("%i\n",uzenetsor);
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     if(uz2<0)
         perror("msgget"); 

     
     child = fork();
     pid_t child2;
     if ( child > 0 ) {
         
          child2=fork();
          if(child2==0)
          {
            sleep(2);
            printf("%s","Itt vagyok");
            kill(getppid(), SIGTERM);

                   struct uzenet uz; 
                    int status; 
                status = msgrcv(uz2, &uz, 1024, 5, 0 ); 
     
                if ( status < 0 ) 
                    perror("msgsnd"); 
                else
                printf( "Az 1. gyerek kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
			sleep(1);
            kill(getppid(), SIGTERM);

     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
         perror("msgsnd"); 
     else
          printf( "A 2. gyerek kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
     return 0; 

          }
          else
          {
                char buffer[100];
				pause();
				read(pipefd[0],buffer,sizeof(buffer)); // reading max 100 chars
				printf("Szulo olvasta uzenet: %s\n\n", buffer);
				sleep(1);

		   pause();
           printf("%s\n","elso gyerek bejelentkezett");
           pause();
           printf("%s\n","masodik gyerek bejelentkezett");

            struct uzenet uz;
            uz.mtype=5; 
            //uz.mtext="Hajra Fradi!"; 
            strcpy(uz.mtext,"Hajra Fradi!");
            int status; 
     
            status = msgsnd( uzenetsor, &uz, sizeof(uz.mtext), 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
                if ( status < 0 ) 
          perror("msgsnd");
        int status2=msgsnd( uz2, &uz, sizeof(uz.mtext), 0 );
                if ( status2 < 0 ) 
                    perror("msgsnd");

            //kuld(uz2,9);
            // Parent sends a message. 
            // After terminating child process, the message queue is deleted. 
            pid_t wpid;
            while ((wpid = wait(&status)) > 0);
                status = msgctl( uzenetsor, IPC_RMID, NULL ); 
                //status2 = msgctl( uz2, IPC_RMID, NULL ); 
            //printf("%i\n",status2);
            if ( status < 0 ) 
               perror("msgctl");
           /*if ( status2 < 0 ) 
               perror("msgctl");*/
            
          }

          return 0; 
     } else if ( child == 0 ) { 
                    sleep(2);
		char buf[100];
		strcpy(buf,argv[1]);
        printf("Gyerek indul!\n");
        write(pipefd[1], buf, sizeof(buf));
        sleep(1);
        kill(child, SIGTERM);


     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
