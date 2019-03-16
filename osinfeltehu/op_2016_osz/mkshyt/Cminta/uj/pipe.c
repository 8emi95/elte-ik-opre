       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	   #include <sys/time.h>
	   #include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

	//
	// unnamed pipe example
	//
	
	
	void handler(int signumber){
		printf("Megerkezett(signal arrived)\n");
	}
	
       int main(int argc, char *argv[])
       {
	char place[100];
    printf("Add meg a helyszint: ");
    fgets(place, 100, stdin);
	char *pos;
    if ((pos=strchr(place, '\n')) != NULL)
        *pos = '\0';  
	
	char place2[100];
    printf("Add meg a kovetkezo programot: ");
    fgets(place2, 100, stdin);
    if ((pos=strchr(place2, '\n')) != NULL)
        *pos = '\0'; 
	   
	   
	   
	   signal(SIGTERM,handler);
	   
		  //int pipefd2[2];
		  int pipefd5[2];
           int pipefd[2]; // unnamed pipe file descriptor array
		   int pipefd3[2];
		   int pipefd4[2];
           pid_t pid;
           char sz[100];
			char sz2[100];		   // char array for reading from pipe
		    char s [15];
		   
			char visitors [5][20];
			strcpy(visitors[0], "elso\n");
			strcpy(visitors[1], "masodik\n");
			strcpy(visitors[2], "harmadik\n");
			strcpy(visitors[3], "negyedik\n");
			strcpy(visitors[4], "otodik\n");
			int number = sizeof(visitors) / 20;
			//printf("%i",number);

           if (pipe(pipefd) == -1) 
	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
           }
		   
		       if (pipe(pipefd3) == -1) 
	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
           }
		          if (pipe(pipefd5) == -1) 
	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
           }
		  	   
		   	      if (pipe(pipefd4) == -1) 
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
	   {		    	// child process
		   sleep(1);
           close(pipefd[1]);  //Usually we close the unused write end
           read(pipefd[0],sz,sizeof(sz)); 
           printf("Gyerek megkapta a helyszint: %s",sz);
	       printf("\n");
		   close(pipefd[0]); // finally we close the used read end
		  
		   printf("A gyerek a helyszinre utazik\n");
		   sleep(2);
		   
		   /*close(pipefd2[0]);
           write(pipefd2[1],"Megerkezett",12);
		   close(pipefd2[1]);
		   */
		
			
			kill(getppid(),SIGTERM); 
			
			
			sleep(1);	
			
			char vis [number][20];
		   //A gyerek megkapja a vendegeket
		   /*
		   close(pipefd3[1]);  //Usually we close the unused write end
           int i;
		   for(i=0;i<=number-1;i++){
		   read(pipefd3[0],sz2,sizeof(sz2)); 
           printf("Gyerek olvasta a vendeget: %s",sz2);
		   strcpy(vis[i], sz2);
	       printf("\n");
		   }
		   close(pipefd3[0]); // finally we close the used read end
			*/
			
		   close(pipefd3[1]);  //Usually we close the unused write end
		   int i;
		
		   read(pipefd3[0],vis,number*20); 
		    printf("Gyerek olvasta a vendegeket: \n");
		   for(i=0;i<=number-1;i++){ 
	       printf("%s",vis[i]);
		   }
		   close(pipefd3[0]); // finally we close the used read end			
		   
		   //Visszaküldi a sikerességet
		   int in=rand()%100,status;
		   char str[15];
		   sprintf(str, "%d", in);
		   close(pipefd4[0]);
           write(pipefd4[1],str,15);
		   close(pipefd4[1]);
		   printf("A sikeresseg elkuldve\n");
		   
		  
			int k;
			char novis[number][20];
			for(k=0;k<=number-1;++k){			
		    srand(time(NULL)); //the starting value of random number generation
			int r=rand()%10; 
			printf("Random number %i\n",r);
			if( r == 1){
				 strcpy(novis[k], visitors[k]);
			} else{
				strcpy(novis[k], " ");
			}			
			sleep(1);
		   }
		    
				 close(pipefd5[0]);
				 write(pipefd5[1],novis,number*20);
				 close(pipefd5[1]);
		  
		   
           } 
           else 
           {    // szulo process 
		 
               close(pipefd[0]); //Usually we close unused read end
               write(pipefd[1], place,100);
               close(pipefd[1]); // Closing write descriptor 
               printf("Szulo kuldi a helyszint!\n");
			   
			  /* close(pipefd2[1]);
			   read(pipefd2[0],sz,sizeof(sz));
			   printf("Szulo megkapta: %s",sz);
			   printf("\n");
			   close(pipefd2[0]); // finally we close the used read end
				*/
				
				pause(); //waits till a signal arrive 
				//printf("Signal arrived\n",SIGTERM);
				
				
				printf("Szulo elkuldi a vendegeket\n");
				/*Ó
				close(pipefd3[0]); //Usually we close unused read end
				int i;
				for(i=0;i<=number-1;i++){
					write(pipefd3[1],visitors[i],20);
					sleep(1);
				}              
			   close(pipefd3[1]); // Closing write descriptor 
			   */
			   
			   close(pipefd3[0]); //Usually we close unused read end
			   write(pipefd3[1], visitors,number*20);           
			   close(pipefd3[1]); // Closing write descriptor 
			   
			   
			   //sleep(2);
			  
			   close(pipefd4[1]);
			   read(pipefd4[0],s,sizeof(s));
			   printf("Szulo megkapta a rendezveny sikeresseget: %s",s);
			   printf("\n");
			   close(pipefd4[0]); 
			   
			   sleep(2);
			   char novis_rec[number][20];
			 
			   close(pipefd5[1]); 
			   read(pipefd5[0],novis_rec,number*20);
			   
			   
			   printf("Nem megerkezett vendegek (ha voltak ilyenek): \n");
			   int k;
			   for(k=0; k<=number-1; k++){
					if(strlen(novis_rec[k]) > 1){
						printf("%s", novis_rec[k]);
					}
			   }
			   
			   printf("\n");
			   close(pipefd5[0]);
			  
			   
			   printf("A kovetkezo helyszin %s", place2 );
			   printf("\n");
			   
			   
			   wait();
               fflush(NULL); 	// flushes all write buffers 
             		
				 };
	   }


 
