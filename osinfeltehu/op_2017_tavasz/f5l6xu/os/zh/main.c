       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	//
	// unnamed pipe example
	//
       int main(int argc, char *argv[])
       {
		   char str[100];
		 //  int asd;
           int pipefd[2]; 
           pid_t pid;
           char sz[100];  

           if (pipe(pipefd) == -1) 
	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
           }
           pid = fork();	
           if (pid == -1) 
	   {
               perror("Fork hiba");
               exit(EXIT_FAILURE);
           }

           if (pid == 0) 
	   {		    	// child process
		  printf("Kérem adja meg a címet! \n");
		   scanf("%s", str);
               close(pipefd[0]);
			   write(pipefd[1], str, sizeof(str));
			   printf("Tehát a cím: %s", str);
			   close(pipefd[1]);
	       printf("\n");
           } 
           else 
           {    // szulo process 
               printf("Központ jelentkezik\n");
               close(pipefd[1]);	
			   read(pipefd[0],sz,sizeof(sz));
			  close(pipefd[0]);
			    printf("Az utas által megadott lakcím: %s \n", sz);
	       printf("Központ vége \n");	
		   wait();	
	   }
	   exit(EXIT_SUCCESS);	
       }

 
