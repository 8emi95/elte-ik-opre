#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> // for errno, the number of last error

#include <sys/ipc.h> 
#include <sys/msg.h>   
#include <string.h>  
#include <wait.h> 

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

// sendig a message
int kuld( int uzenetsor , char* s) 
{ 
     struct uzenet uz = { 5, "Hajra Fradi!" }; 
	 //uz.mtext="asd";
	 sprintf(uz.mtext, "%s ", s);
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd");
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

int main(int argc, char* argv[])
{
    int pid,fd;
    printf("Pamacs start!\n");
    int fid=mkfifo("pamacs", S_IRUSR|S_IWUSR ); // creating named pipe file
    // S_IWGRP, S_IROTH (other jog), file permission mode
    // the file name: fifo.ftc
    // the real fifo.ftc permission is: mode & ~umask 
    if (fid==-1)
    {
	printf("Error number: %i",errno);
	exit(EXIT_FAILURE);
    }
    printf("Mkfifo vege, fork indul!\n");
    
	pid = fork();
    
    if(pid==0)   //child
	{
		int i;
	 //printf("Number of command line arguments are: %i\n",argc);
	 char s[1024]="";	
	 for (i=1;i<argc;i++){
	  //printf("%i. argument is %s\n",i,argv[i]);
	  sprintf(s, "%s %s",s, argv[i]);
	 }
	 //printf("%s",s);
	 sprintf(s, "%s %i",s, getppid());
	printf("Gyerek vagyok, irok a csobe!\n");
	printf("Csonyitas eredmenye: %d!\n",fid);
        fd=open("pamacs",O_WRONLY);
        write(fd,s,1024);
        close(fd);
	printf("Gyerek vagyok, beirtam, vegeztem!\n");
        }
	else // Parent
	{	
	char s[1024]="Semmi";		
	printf("Csonyitas eredmenye szuloben: %d!\n",fid);
	fd=open("pamacs",O_RDONLY);
	read(fd,s,sizeof(s));
	printf("Ezt olvastam a csobol: %s \n",s);
	close(fd);
	if(s!=""){
		pid_t szulo; 
		 int uzenetsor, status; 
		 key_t kulcs; 
		 
		 // msgget needs a key, amelyet az ftok generál 
		 //. 
		 kulcs = ftok(argv[0],1); 
		 printf ("A kulcs: %d\n",kulcs);
		 uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
		 if ( uzenetsor < 0 ) { 
			  perror("msgget"); 
			  return 1; 
		 } 
     
		szulo = fork();
		if(szulo<0){
			printf("baj van a masodik forkal\n");
		}
		if(szulo==0){//gyerek
			printf("Taxis jeno keszen all!!\n");
			return fogad( uzenetsor ); 
          // The child process receives a message. 
		}else{//szulo
			//printf("Taxis jeno meghalt!!\n");
			kuld( uzenetsor,"haho lali" );  // Parent sends a message. 
			  wait( NULL ); 
			  // After terminating child process, the message queue is deleted. 
			  status = msgctl( uzenetsor, IPC_RMID, NULL ); 
			  if ( status < 0 ) 
				   perror("msgctl"); 
			  return 0; 
		}		
		
		
	}
	// remove fifo.ftc

	wait( NULL ); 
	unlink("pamacs");
	}	
    
    return 0; 
}
