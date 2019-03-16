#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h> 
#include <errno.h> // for errno, the number of last error
#include <wait.h> 

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char* mtext; 
}; 

// sendig a message
int kuld( int uzenetsor, char* uzi ) 
{ 
     const struct uzenet uz = { 3630123456789, uzi }; 
	 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, sizeof(uz.mtext) , 0 ); 
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
     status = msgrcv(uzenetsor, &uz, 1024, 3630123456789, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A taxi fogadja az uzenetet: Telefonszam: %ld, Cim:  %s\n", uz.mtype, uz.mtext ); 
     return 0; 
} 

int main (int argc, char* argv[]) { 
     pid_t child; 
	 pid_t child1;
	 
	 char* cim=argv[1];	
	 
	 
     int uzenetsor, status; 
     key_t kulcs; 
     
     // msgget needs a key, amelyet az ftok generál 
     //. 
     kulcs = ftok(argv[0],1); 
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( IPC_PRIVATE, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
	 
	int fd,f1; 
	printf("Fifo start!\n");
    int fid=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR ); // creating named pipe file
    // S_IWGRP, S_IROTH (other jog), file permission mode
    // the file name: fifo.ftc
    // the real fifo.ftc permission is: mode & ~umask 
    if (fid==-1)
    {
		printf("Error number: %i",errno);
		exit(EXIT_FAILURE);
    }
    printf("Mkfifo vege, fork indul!\n");
	 
	 
	 
	 child1 = fork();
	 
	 if ( child1 > 0 ){
		 
		printf("Csonyitas eredmenye szuloben: %d!\n",fid);
		fd=open("fifo.ftc",O_RDONLY);
		read(fd,cim,sizeof(cim));
		printf("Ezt olvastam a csobol: %s \n",cim);
		close(fd);
		
		unlink("fifo.ftc"); 
		 
		 
		 
		child = fork(); 
		 if ( child > 0 ) { 
			  kuld( uzenetsor, cim );  // Parent sends a message. 
			  wait( NULL ); wait( NULL );
			  // After terminating child process, the message queue is deleted. 
			  status = msgctl( uzenetsor, IPC_RMID, NULL ); 
			  if ( status < 0 ) 
				   perror("msgctl"); 
			  return 0; 
		 } else if ( child == 0 ) { 
			  return fogad( uzenetsor ); 
			  // The child process receives a message. 
		 } else { 
			  perror("fork"); 
			  return 1; 
		 }  
		 wait( NULL );
	 }else{
		printf("Gyerek vagyok, irok a csobe!\n");
		printf("Csonyitas eredmenye: %d!\n",fid);
        fd=open("fifo.ftc",O_WRONLY);
        write(fd,cim,sizeof(cim));
        close(fd);
		printf("Gyerek vagyok, beirtam, vegeztem!\n");
	 }
     
     
     
     return 0; 
} 