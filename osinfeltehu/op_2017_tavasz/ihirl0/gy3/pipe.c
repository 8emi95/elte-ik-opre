       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	//
	// unnamed pipe example
	//
		struct Kerdes
		{
			char Q[30];
			char A1[20];
			char A2[20];
			char A3[30];
			char A4[30];
		};
       int main(int argc, char *argv[])
       {
			
           int pipefd[2]; // unnamed pipe file descriptor array
           pid_t pid;
           char sz[1];  // char array for reading from pipe
		   char text[2];
		   struct Kerdes kerdesek[10];
		   
		  // printf(text);

           if (pipe(pipefd) == -1) 
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
	       sleep(3);	// sleeping a few seconds, not necessary
               close(pipefd[1]);  //Usually we close the unused write end
               read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
			   if(atoi(&sz[0])== 1)
			   {
				   printf("adja meg a kerdest(szokozok nelkul) \n");
				   scanf("%s",kerdesek[0].Q);
				   printf("adja meg az 1. valaszt(szokozok nelkul) \n");
				   scanf("%s",kerdesek[0].A1);
				   printf("adja meg az 2. valaszt(szokozok nelkul) \n");
				   scanf("%s",kerdesek[0].A2);
				   printf("adja meg az 3. valaszt(szokozok nelkul) \n");
				   scanf("%s",kerdesek[0].A3);
				   printf("adja meg az 4. valaszt(szokozok nelkul) \n");
				   scanf("%s",kerdesek[0].A4);
			   }
			   else
			   {
				   printf("butavagy\n");
			   }
			   fflush(stdout); 
	       printf("\n");
               close(pipefd[0]); // finally we close the used read end
           } 
           else 
           {    // szulo process 
				printf("1-Uj kerdes felvetele");
				printf("2-Kerdoiv kitoltese");
				scanf("%s",text);
           
               close(pipefd[0]); //Usually we close unused read end
               write(pipefd[1], &text,2);
               close(pipefd[1]); // Closing write descriptor 
			 
               	// flushes all write buffers (not necessary)
               wait();		// waiting for child process (not necessary)
				// try it without wait()
	       printf("Szulo befejezte!");	
	   }
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }

 
