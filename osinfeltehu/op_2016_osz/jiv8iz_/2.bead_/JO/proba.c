#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
 
void handler(int signumber){
	printf("SIGNAL: Megerkeztem a helyszinre.\n");
}

void handler2(int signumber){
	printf("SIGNAL: Megkaptam a jelentkezoket.\n");
}	

void handler3(int signumber){
	srand(time(NULL));
	int siker = rand() %101;
	printf("\nSIGNAL: A rendezveny lebonyolitva.Sikeressege: %i \n", siker);
}	

int main(int argc, char *argv[])
{		
		char jsz[5]    ;
		int jelentkezokszama,rendezvenyekszama;
		char helyszin[100];
	
		char ujnev[100];
		int a;
		
		printf("Adja meg a rendezvenyek szamat: ");
		scanf("%s",jsz);
		rendezvenyekszama = atoi(jsz);
		
		char rendezvenyek[rendezvenyekszama][10];		
		for(a=0;a<rendezvenyekszama;a++){
	
			printf("Adja meg az %i. rendezveny nevet:  ",a+1);
			scanf("%s",ujnev);
			strcpy(rendezvenyek[a],ujnev);
		}
				
		printf("Adja meg a helyszin nevet: ");
		scanf("%s",helyszin);
		printf("Adja meg hany jelentkezo lesz: ");
		scanf("%s",jsz);
		jelentkezokszama = atoi(jsz);
				
		char nevek[jelentkezokszama][10];
		for(a=0;a<jelentkezokszama;a++){
	
			printf("Adja meg az %i. jelentkezo nevet:  ",a+1);
			scanf("%s",ujnev);
			//printf("Olvasott: %s",ujnev);
			strcpy(nevek[a],ujnev);
		}
		
		signal(1,handler);
		signal(2,handler2);
		signal(3,handler3);

        int pipefd[2]; // unnamed pipe file descriptor array
		int pipefd2[2];
		
        pid_t pid;
        char sz[100];  // char array for reading from pipe
		
		if(mkfifo("fifo",0666) == -1){
			perror("Fifo hiba");
		};
		
		
        if (pipe(pipefd) == -1  || pipe(pipefd2) == -1) 
	    {
            perror("Hiba a pipe nyitaskor!");
            exit(EXIT_FAILURE);
        }
       
       pid = fork();	// creating parent-child processes
       if (pid == -1) 
	   {
            perror("Fork hiba");
            exit(EXIT_FAILURE);
        }

       if (pid == 0) 
	   {// child process
	       sleep(3);	// sleeping a few seconds, not necessary
           close(pipefd[1]);  //Usually we close the unused write end
	       printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
           read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
           printf("Gyerek olvasta uzenet: %s \n",sz);
	       printf("A gyerek a helyszinre utazik.\n");
		   sleep(2);
	
		   kill(getppid(),1);
           close(pipefd[0]); // finally we close the used read end
		   printf("Gyerek varja a jelentkezok szamat!\n");
		   int fd = open("fifo", O_RDONLY);
		   
		   char nev[100];
		   int i;
		   for( i=0;i<jelentkezokszama;i++){
				read(fd,nev,sizeof(nev));
				printf("Fogadott nev: %s \n", nev);
		   }
		   
		   kill(getppid(),2);
		   printf("Gyerek lebonyolitja a rendezvenyt! :)\n");
		   sleep(3);
		   
		   kill(getppid(),3);
		   close(fd);
		   
		   char nincs[100];
		   int r = 0;
		   int j;
		   srand(time(NULL));
		   for(j =0;j<jelentkezokszama;j++){
			r = rand() % 10;
			printf("%i ",r);
			if (r == 1){				
				strcpy(nincs,nevek[j]);
			    printf("Nem jelent meg a kovetkezo: %s\n",nincs);
			}
		   }
		  
	
		  write(pipefd2[1],nincs, sizeof(nincs));
		  close(pipefd2[1]);
		   
        } 
        else 
        { // szulo process 
            printf("Szulo indul!\n");
            close(pipefd[0]); //Usually we close unused read end
            write(pipefd[1], helyszin,sizeof(helyszin));
            close(pipefd[1]);  
            printf("Szulo beirta az adatokat a csobe!\n");
   			pause(); // vár a signal megérkezéseére
			printf("A szignal megerkezett, a gyerek a helyszinen van.\n");
			printf("Szulo kuldi a jelentkezoket.\n");
			printf("Jelentkezok szama: %i \n", jelentkezokszama);
			int fd = open("fifo", O_WRONLY);
			   
			char aktnev[100];
			int i;
			for( i=0;i<sizeof(nevek);i++){
				strcpy(aktnev,nevek[i]);
				write(fd,aktnev,sizeof(aktnev));
			}
			   
			printf("Szulo varja a rendezveny lebonyolitasat.\n");
			pause();
		    close(fd);
			
			char nincs[100];
			read(pipefd2[0],nincs,sizeof(nincs));
			
			printf("Kapott nev, aki nem jelent meg: %s \n",nincs);
			
			
			
			for(i=0;i<rendezvenyekszama;i++){
				printf("%i. rendezveny helyszine: %s\n",i+1,rendezvenyek[i]);
			}
			
			for(i=0;i<rendezvenyekszama;i++){
				if(strcmp(rendezvenyek[i],helyszin) == 0){
					printf("A kovetkezo helyszin: %s\n", rendezvenyek[i+1]);
					if(i == rendezvenyekszama-1) { printf("Nincsen kovetkezo helyszin\n");}
				}
			}
			close(pipefd2[0]);
     		unlink("fifo");
			
            fflush(NULL); 	// flushes all write buffers (not necessary)
            wait();		// waiting for child process (not necessary)
				
	       printf("Szulo befejezte!\n");	
		}
	   exit(EXIT_SUCCESS);	// force exit, not necessary
}

