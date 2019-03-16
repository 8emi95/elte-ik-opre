#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <regex.h>
#include <wait.h>

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

// sendig a message
int kuld( int uzenetsor) 
{ 
    const char *jokivansagok[] = {"\n\n Vigyazzatok magatokra!\n\n", "\n\n Mindorokke Kiraly!\n\n", "\n\n Hurra!\n\n", "\n\n Zsiraf!\n\n"};
    //sleep(1);
    int random = rand()%4;
    struct uzenet uz = { 5, "Hajra Fradi!" };
    strcpy(uz.mtext, jokivansagok[random]);
 	 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A kivansag:  %s\n", uz.mtext ); 
     return 0; 
} 


void handler(int signumber){
    printf("Jelzes kuldese megtortent\n",signumber);
}

int main (int argc, char* argv[]) { 
     pid_t child = fork();
	pid_t child2;	
     int uzenetsor, status; 
     key_t kulcs; 
	 int pipefd[2];
	 int pipefd2[2];
	 int betoro = -1;
	 int riasztas = 1;
	 int riaszt2 = riasztas;
	 
	  //csohoz
    char sz1[100];
    char sz2[100];
	char sz3[100];
	char sz4[100];
	
	    if (pipe(pipefd) == -1)
    {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }
     
     // msgget needs a key, amelyet az ftok generál 
     //. 
     kulcs = ftok(argv[0],1); 
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
	 
	 signal(SIGTERM, handler);
     
      
	 

		if(child ==0 ){
			printf("Az 1. jarorauto vegzett a reggelivel es jelzest kuldott a kozpontnak\n");
            kill(getppid(), SIGTERM);
			fogad(uzenetsor);
			

			read(pipefd[0],sz2,sizeof(sz2)); // reading max 100 chars
			
			printf("Child 1 : %i", atoi(sz2));
			
			if(atoi(sz2) == 0){
				printf("(rendor 1) Nincs riasztas\n");
			}else{
				printf("(rendor 1) Riasztas van\n");
				printf("Az 1. jaror elindult a helyszinre\n");
				
				sleep(1);

				if(betoro == -1){
					//betoro = rand()%10;
					betoro = 8;
				}
				
				if(betoro>=6){
					printf("(rendor 1)Talaltunk betorot\n");
					char flag[2] = "y";
			//		write(pipefd[1], flag, 100);

				}else{
					printf("(rendor 1)Nem talaltunk betorot\n");
					char flag[2] = "n";
			//		write(pipefd[1], flag, 100);
	
				}

	 }
		}
		else if(child2 == 0){
			printf("A 2. jarorauto vegzett a reggelivel es jelzest kuldott a kozpontnak\n");
			kill(getppid(), SIGTERM);
			
			fogad(uzenetsor);
			

			read(pipefd[0],sz4,sizeof(sz4)); // reading max 100 chars
			
			printf("Child 2 : %i", atoi(sz4));
			
			if(atoi(sz4) == 0){
				printf("(rendor 2) Nincs riasztas\n");
			}else{
				printf("(rendor 2) Riasztas van\n");
				printf("A 2. jaror elindult a helyszinre\n");

				if(betoro == -1){
					//betoro = rand()%10;
					betoro = 8;
				}
				if(betoro>=6){
					printf("(rendor 2)Talaltunk betorot\n");
					char flag[2] = "y";
			//		write(pipefd2[1], flag, 100);
				}else{
					printf("(rendor 2)Nem talaltunk betorot\n");
					char flag[2] = "y";
			//		write(pipefd2[1], flag, 100);
				}

				
			
			
			}
		}
		
		
    else { 
	
		child2 = fork();
		
        //  kuld( uzenetsor );  // Parent sends a message. 
			pause();
			
			
			printf("A jarorautok tudattak hogy keszen allnak a munkara\n");
			
			 kuld(uzenetsor);
			 kuld(uzenetsor);

			 sleep(3);
			 printf("Kezdodik a napi rutin!\n");
			 
			 
			
			
			char elkuldeni[10];
			sprintf(elkuldeni,"%i",riasztas);
				

			 if(riasztas == 0){
				 printf("Nem volt riasztas!\n");
				 write(pipefd[1], elkuldeni, 100);
				 
			 } else {
				 
				 write(pipefd[1], elkuldeni, 100);
				 
				 
						 
			//	 read(pipefd[0], sz1, sizeof(sz1));
			//	 read(pipefd2[0], sz3, sizeof(sz3));
				 
			//	 printf("Child 1 : %s", sz1);
			//	 printf("Child 2 : %s", sz3);

				
			
			 }
			
			           wait(NULL);
            status = msgctl( uzenetsor, IPC_RMID, NULL );
            if ( status < 0 ) perror("msgctl");
 
            int status;
            wait(&status);
            printf("A kozpont befejezte a mukodeset, kezdodhet a kaosz\n");
			
		}
		
     
	
	return 0;
} 