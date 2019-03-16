       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	//
	// unnamed pipe example
	//
	
	struct adatok
	{
		int tag1;
		int tag2;
		char muvelet;
	};
       int main(int argc, char *argv[])
       {
		   if (argc<4) {printf("Keves argumentum!\n"); exit(1);}
           int pipefd[2]; // unnamed pipe file descriptor array (1. csohoz)
           int pipefd2[2]; // unnamed pipe file descriptor array (2. csohoz)
           pid_t pid;
           char sz[100];  // char array for reading from pipe

           if (pipe(pipefd) == -1) // 1. cso megnyitasa
	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
           }
           if (pipe(pipefd2) == -1) // 2. cso megnyitasa
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
	       //sleep(3);	// sleeping a few seconds, not necessary
               //close(pipefd[1]);  //Usually we close the unused write end
	       printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
		       //***** karakterkonstans kuldese a csovon *****
               //read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
               //printf("Gyerek olvasta uzenet: %s",sz);
               //***** egesz szaz kuldese a csovon *****
               int x1, y1, eredmeny;
               char *z1;
               read(pipefd[0],&x1,sizeof(int)); // reading max 100 chars
               read(pipefd[0],&y1,sizeof(int)); // reading max 100 chars
               read(pipefd[0],&z1,sizeof(char*)); // reading max 100 chars
               printf("Gyerek olvasta uzenet: %i\n",x1);
               printf("Gyerek olvasta uzenet: %i\n",y1);
               printf("Gyerek olvasta uzenet: %s\n",&z1);
               
               /*if(!strcmp("-", z1))
               {
				   eredmeny = x1-y1;
			   }
               else if(!strcmp("+", z1))
               {
				   eredmeny = x1+y1;
			   }
               else if(!strcmp("*", z1))
               {
				   eredmeny = x1*y1;
			   }
			   else
			   {
				   eredmeny = 0;
			   }*/
			   
			   eredmeny = x1*y1;
               
               write(pipefd2[1], &eredmeny,sizeof(int));
               
               //*********************************************
	       printf("\n");
               close(pipefd[0]); // finally we close the used read end
           } 
           else 
           {    // szulo process 
               printf("Szulo indul!\n");
               //close(pipefd[0]); //Usually we close unused read end
               //***** szamolasi adatok elcsovezese *****
               int x = atoi(argv[1]);
               int y = atoi(argv[2]);
               int eredm;
               char *z = argv[3];
               write(pipefd[1], &x,sizeof(int));
               write(pipefd[1], &y,sizeof(int));
               write(pipefd[1], z,sizeof(char*));
               
               //***** egesz szaz kuldese a csovon *****
               /*const int x = 25;
               write(pipefd[1], &x,sizeof(int));*/
               //***** karakterkonstans kuldese a csovon *****
               /*write(pipefd[1], "Hajra Fradi!",12);
               write(pipefd[1], "Hajra MTK!",50);*/
               //*********************************************
               //close(pipefd[1]); // Closing write descriptor 
               printf("Szulo beirta az adatokat a csobe!\n");
               read(pipefd2[0],&eredm,sizeof(int)); // reading max 100 chars
               printf("Szulo olvasta uzenet: %i\n",eredm);
               fflush(NULL); 	// flushes all write buffers (not necessary)
               wait();		// waiting for child process (not necessary)
				// try it without wait()
	       printf("Szulo befejezte!");	
	   }
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }

 
