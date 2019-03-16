#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> // for errno, the number of last error
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 



struct uzenet { 
     int mtype;//uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 
void handler(int signumber);
int kuld( int uzenetsor , struct uzenet *uz);
int fogad( int uzenetsor); 

int main(int argc, char* argv[]){
    int pid,fd;
	unlink("fifo.ftc");
    int fid=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR );
	signal(SIGUSR1,handler);
    if (fid==-1)
    {
		printf("Error number: %i",errno);
		exit(EXIT_FAILURE);
    }
    pid = fork();
    
    if(pid>0)   //parent 
	{
		
		pid_t pid2 = fork();
		if (pid2 > 0){
			pause();
			printf("Signal 1 megjott: %d \n",SIGUSR1);
			char s[1024]="ures";	
			fd=open("fifo.ftc",O_RDONLY);
			read(fd,s,sizeof(s));
			printf("Kozpont : Ezt olvastam a csobol: %s \n",s);
			
		}else{ //child2
			//ide kene meg majd valami telszam felesegt
		}
    }
	else // child
	{
		printf("varunk 3 masodpercet, majd kuldjuk a sigtermet %i signal\n",SIGUSR1);
		sleep(1);
		kill(getppid(),SIGUSR1);
		printf("Utas vagyok hivom a taxi tarsosagot!\n");
        fd=open("fifo.ftc",O_WRONLY);
        write(fd,argv[1],1024);
        close(fd);
		exit(0);
		
    }	
    
    return 0; 
}
 
 void handler(int signumber){
	printf("telcsizunk!\n");
}

int kuld( int uzenetsor , struct uzenet *uz) { 
     
     int status; 
     
     status = msgsnd( uzenetsor, uz, strlen ( uz->mtext ) + 1 , 0 ); 
		// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
}

int fogad( int uzenetsor) { 
     struct uzenet uz; 
     int status; 
    
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgrcv"); 
     else
          printf( "A kapott telefonszam: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
     return 0; 
} 