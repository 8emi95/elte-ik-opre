#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 
#include <signal.h>
#include <sys/types.h>
#include <unistd.h> // for pipe()
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <string.h> 
#include <wait.h> 

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

// sendig a message
int kuld( int uzenetsor ) 
{ 
     const struct uzenet uz = { 5, "Hajra Fradi!" }; 
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
          printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
     return 0; 
} 

void handler(){
  printf("Signal has arrived\n");
}

int main(int argc,char** argv)
{
int status;
int* notacommonvalue = (int *) malloc(sizeof(int));
int* expediciok = (int *) malloc(sizeof(int));
int* ido = (int *) malloc(sizeof(int));

*expediciok = atoi(argv[1]);
*ido = atoi(argv[2]);

const char *megfigyelesek[3];
megfigyelesek[0] = "sanyi budapest 3 idos 2013.03.02";
megfigyelesek[1] = "bela budapest 4 fiatal 2013.02.02";
megfigyelesek[2] = "zoli dunakeszi 3 fiatal 2015.03.19";
 
const char *a[5];
a[0] = "medve";
a[1] = "farkas";
a[2] = "oroszlan";
a[3] = "tigris";
a[4] = "roka";

/*signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
*/

int i;
pid_t pid;

for(i = 0; i < *expediciok; i++) {
	int pipefd[4]; // unnamed pipe file descriptor array
	char sz[100];  // char array for reading from pipe 
	pipe(pipefd);
	int uzenetsor, status; 
    key_t kulcs; 
	kulcs = ftok(argv[0],1); 
    uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
    if ( uzenetsor < 0 ) { 
        perror("msgget"); 
        return 1; 
    } 
    pid = fork();

    if(pid < 0) {
        perror("Fork hiba");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
		sz[0] = '\0';
		printf("Gyerek #%i kesz! Indulhat a munka!\n", i+1);
		sleep(1);	// sleeping a few seconds, not necessary
        close(pipefd[1]);  //Usually we close the unused write end
		printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
        read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
        printf("%s azonositojo gyerek megkapta az uzenetet es kesz!\n",sz);
		printf("\n");
        close(pipefd[0]); // finally we close the used read end
		
		//kezdjuk a megfigyelest
		srand(time(NULL));
		int randomnumber;
		randomnumber = 2 + rand() % 4;
		printf("Alvas %d masodpercig...zzzz\n", randomnumber);
		if(randomnumber == 5){
			printf("5 mp-es varakozas miatt visszahivas!\n", randomnumber);
			exit(0);
		} else {
			printf("Megfigyelesek kuldese:\n");
		}
		sleep(randomnumber);
		int j;
		for(j = 0; j<3; j++){
			printf("%s , ", megfigyelesek[j]);
		}	
        exit(0); 
		
    } else  {
        //printf("Szulo indul!\n");
        close(pipefd[0]); //Usually we close unused read end
        write(pipefd[1], a[i],10);
        close(pipefd[1]); // Closing write descriptor 
        //printf("Szulo beirta az adatokat a csobe!\n");
        fflush(NULL); 	// flushes all write buffers (not necessary)
        wait(NULL);		// waiting for child process (not necessary)
		// try it without wait()
	    printf("Szulo befejezte!\n");	
    }
}

printf("Szulo megszerzett minden adatot! Adatok lementve!\n");

return 0;
}