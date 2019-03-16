/** /
       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	//
	// unnamed pipe example
	//
       int main(int argc, char *argv[])
       {
           int pipefd[2]; // unnamed pipe file descriptor array
           pid_t pid;
           char sz[100];  // char array for reading from pipe
           int n, status;
               char str1[]="halika";
    char str2[]="Juh\n";


           if (pipe(pipefd) == -1)
    	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
           }
           pid = fork();	// creating parent-child processes
           if (pid == -1) {
               perror("Fork hiba");
               exit(EXIT_FAILURE);
           }

           if (0 == pid) {
               // child process
               close(pipefd[1]);  //Usually we close the unused write end
               while (n = read(pipefd[0], sz, sizeof(sz)) > 0) { // Read until it returns 0 (EOF) or -1 (error)
                  printf("Gyerek olvasta uzenet 1: %s", sz);
                  printf("\n");
                  fflush(NULL);
               }
               if (n < 0) { // -1 = error
                   perror("read from pipe");
               }
               /** /
	           printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               read(pipefd[0], sz, sizeof(sz)); // reading max 100 chars
               printf("Gyerek olvasta uzenet 1: %s\n", sz);
               read(pipefd[0], sz, sizeof(sz)); // reading max 100 chars
               printf("Gyerek olvasta uzenet 2: %s\n", sz);
               /** /
               close(pipefd[0]); // finally we close the used read end
           } else {
               // szulo process 
               close(pipefd[0]); //Usually we close unused read end
               write(pipefd[1], "str1", 4);
               write(pipefd[1], "str2", 4);
               close(pipefd[1]); // Closing write descriptor 

               printf("Szulo beirta az adatokat a csobe!\n");
               //fflush(NULL); 	// flushes all write buffers (not necessary)
               wait(&status);
				// try it without wait()
	           printf("Szulo befejezte!\n\n");
    	   }
	       exit(EXIT_SUCCESS);	// force exit, not necessary
       }
/**/
 



/** /
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 50

void main(){

    int fd[2], n, status;
    char buf[MAX];
    pid_t pid;
    char str1[]="ello World!\n";
    char str2[]="Goodbye\n";
    pipe(fd);

    if((pid=fork()) < 0){
        abort();
    }

    else if(pid > 0){// parent code goes here
        close(fd[0]); // close read channel of pipe in parent
        write (fd[1], str1, strlen(str1)+1); // write "hello world"
        //write (fd[1], str2, strlen(str2)); // write "goodbye"
        close(fd[1]); // Tell child that we're done writing

        wait(&status); // Wait for child to read everything and exit
    }
    else { // child code goes here
        close(fd[1]); // close write channel of pipe in child
        while (n = read(fd[0], buf, sizeof(buf)) > 0) { // Read until it returns 0 (EOF) or -1 (error)
            write(STDOUT_FILENO, buf, n);
            printf("  juh %s\n", buf);
            fflush(stdout);
        }
        if (n < 0) { // -1 = error
            perror("read from pipe");
        }
    }
}
/**/





/**/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int     fd[2], nbytes;
    pid_t   childpid;
    char    string[] = "Hello, world!\n";
    char    string2[] = "Goodbye!\n";
    char    readbuffer[80];

    pipe(fd);
    
    if((childpid = fork()) == -1) {
        perror("fork");
        exit(1);
    }

    if(childpid == 0) {
        // Child process closes up input side of pipe
        close(fd[0]);

        // Send "string" through the output side of pipe 
        write(fd[1], string, (sizeof(string)));
        write(fd[1], string2, (sizeof(string2)));
        exit(0);
    } else {
        // Parent process closes up output side of pipe
        close(fd[1]);

        /** /
        while (read(fd[0], readbuffer, sizeof(readbuffer)) > 0) {
            //nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
            printf("Received string: %s", readbuffer);
        }
        /**/
        // Read in a string from the pipe
        nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
        printf("Received string: %s", readbuffer);
        // Read in a string from the pipe
        nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
        printf("Received string: %s", readbuffer);
        /**/
    }
    
    return(0);
}
/**/