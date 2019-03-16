       #include <sys/wait.h>
//       #include <assert.h>
       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h>
       #include <string.h>

       int main(int argc, char *argv[])
       {
           int i,pipefd[2];
           pid_t cpid;
           char buf;

           // assert(argc == 2);
	   if (argc==1)
	    {
	     perror("Hiba, nincs parameter!");
	     exit(1);
	    }
           if (pipe(pipefd) == -1) {
               perror("pipe");
               exit(EXIT_FAILURE);
           }

           cpid = fork();
           if (cpid == -1) {
               perror("fork hiba");
               exit(EXIT_FAILURE);
           }

           if (cpid == 0) {    /* gyerek olvas a pipe-bol */
	       sleep(5);
               close(pipefd[1]);          /* Close unused write end */
	       printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               while (read(pipefd[0], &buf, 1) > 0)
                   printf("%c",buf);
                   //write(STDOUT_FILENO, &buf, 1);
	       printf("\n");
               //write(STDOUT_FILENO, "\n", 1);
               close(pipefd[0]);
               _exit(EXIT_SUCCESS);

           } 
           else 
           {    /* szulo az argv parametereket pipe-ba irja */
               printf("Szulo indul!\n");
               close(pipefd[0]);          /* Close unused read end */
               for(i=1;i<argc;i++)
                {
                write(pipefd[1], argv[i], strlen(argv[i]));
                write(pipefd[1],"\t",1);
                }
               close(pipefd[1]);          /* Az olvaso EOF-t lat! */
               printf("Szulo beirta az adatokat a csobe, var a gyerek vegere!\n");
               fflush(NULL);
              // wait(NULL);                /* Wait for child */
               exit(EXIT_SUCCESS);
           }
       }

 