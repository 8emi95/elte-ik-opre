#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> // for errno, the number of last error
	//
	// unnamed pipe example
	//
void handler(int signumber){
	printf("SIGNAL: Megerkeztem a helyszinre.\n");
}
	
int main(int argc, char *argv[])
{
		// Deklaráció
	    signal(SIGTERM,handler);
	    
		pid_t pid;
	    pid = fork();	// creating parent-child processes
        
		int pipefd[2]; // unnamed pipe file descriptor array
        char sz[100];  // char array for reading from pipe
		   
		
		int fid,fd;
		fid= mkfifo ("fifo.ftc", S_IRUSR | S_IWUSR  );; // creating named pipe file

		
		char nevek[5][100] = {"elso","masodik","harmadik","negyedik","otodik"};
		
		// Ellenőrzések
		if (pid == -1) 
	    {
               perror("Fork hiba");
               exit(EXIT_FAILURE);
        }
		
		if(fid == -1)
		{
			printf("Hiba a fifo megnyitasanal: ");
			exit(EXIT_FAILURE);
		}
		
        if (pipe(pipefd) == -1) 
	    { 
			perror("Hiba a pipe nyitaskor!");
            exit(EXIT_FAILURE);
        }
        
        
		
		
		// Szülő- gyerek kapcsolat
       if (pid == 0) 
	   {		    	// child process
		int vendegek=0;
	       sleep(3);	// sleeping a few seconds, not necessary
               close(pipefd[1]);  //Usually we close the unused write end
	       printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
               printf("Gyerek megkapta a helyszint: %s",sz);
	       printf("\n");
				printf("A gyerek a helyszinre utazik.\n");
				sleep(2);
				printf("A helyszinen vagyok. :)\n");
				kill(getppid(),SIGTERM);
				printf("Varom a jelentkezoket: \n\n");
				close(pipefd[0]); // finally we close the used read end
				
				char kapottnev[10];
				fd = open("fifo.ftc",O_RDONLY);
				read(fd,kapottnev,sizeof(kapottnev));
				printf("A kapott nev: &s \n", kapottnev);
				close(fd);
				
				unlink("fifo.ftc");
				
               close(pipefd[0]); // finally we close the used read end
        } 
        else 
        {    // szulo process 
               printf("Szulo indul! Elkuldi a helyszin nevet a gyereknek.\n");
               close(pipefd[0]); //Usually we close unused read end
               write(pipefd[1], "Budapest",9);
               //close(pipefd[1]); // Closing write descriptor 
               printf("Szulo beirta a helyszint!\n");
			   //wait();			   
			   pause(); // vár a signal megérkezéseére
			   printf("A szignal megerkezett, eljutott a helyszinre a gyerek.\n");
               //fflush(NULL); 	// flushes all write buffers (not necessary)
              // wait();		// waiting for child process (not necessary)
				// try it without wait()
				
				/*
				write(pipefd[1],"Kiss Mari",10);
				//close(pipefd[1]);
				printf("Elkuldte az elso jelentkezo nevet: Kiss Mari\n");
				
				write(pipefd[1],"Nagy Kati",10);
			    //close(pipefd[1]);
				write(pipefd[1],"Kovacs Jozsi",13);
				close(pipefd[1]);
				*/
				
				fd = open("fifo.ftc",O_WRONLY);
				//write(fd,&nevek, sizeof(nevek));
				//char nev[10] = "Elso";				
				write(fd,"Elso",5);
				close(fd);
				printf("\nBeirtam a fajlba a neveket! \n");
				wait();
	       printf("Szulo befejezte!");	
	    }
	  
     exit(EXIT_SUCCESS);	// force exit, not necessary
}

 
