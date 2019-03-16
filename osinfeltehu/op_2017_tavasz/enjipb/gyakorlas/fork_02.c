#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

/************
Ez a kód azt demonstrálja, hogy hogyan kell egy szülőből két gyereket leszármaztatni. Szülő a gyökér a gyerekek pedig a levelek.
Fa szerkezetben.
*************/

/*Functions*/
void handler(int signumber);



int main(int argc, char *argv[])
{
    /*General variables and data structures*/
    int rv;
   
    /* create signal */
    signal(SIGTERM,handler);

    /* create pipe */
    int pipefd[2]; // unnamed pipe file descriptor array
    pid_t child1, child2; // Child processes: Student1 and Stundent2
    char buffer[5][255];  // char array for reading from pipe
    int  pidIDs[3]; //0: parent 1:Child1 2:Child2
    int status;


    if (pipe(pipefd) == -1) {
        perror("Error: Opening the Pipe");
        exit(EXIT_FAILURE);
    }

    /* creating parent-child processes */
    child1 = fork();

    if (child1 == -1) {
        perror("Error: Fork -- CHILD1\n");
        exit(EXIT_FAILURE);
    }

    /* parent process */
    if (0 != child1)    
    {   
        printf("[ PARENT ]: This is the parent process!\n");
        printf("[ PARENT ]: I am the parent my pid is %d\n", getpid());
        
        child2 = fork();
        if (child2 == -1) {
            perror("Error: Fork -- CHILD2\n");
            exit(EXIT_FAILURE);
        }
        if(child2 == 0){
            printf("[ CHILD2 ]: I am the SECOND child. My PID: %d, and my parent's PID: %d\n", getpid(), getppid());
            
            /*Here DO what CHILD2 needs to do!! */
            
            printf("[ CHILD2 ]: FINISHED\n");
        }

        /*Here DO what PARENT needs to do!! */

      
        wait(&status);
        /*  OR
            waitpid(child1, &status, 0);
            waitpid(child2, &status, 0);
        */
	    printf("[ PARENT ]: Parent has finished!\n");	
    }
    /* child process */
    else if( child1 == 0){
           printf("[ CHILD1 ]: I am the FIRST child. My PID: %d, and my parnet's PID: %d\n", getpid(), getppid());
           
           /*Here DO what CHILD1 needs to do!! */
           
           printf("[ CHILD1 ]: FINISHED\n");
    }
    
    return 0;
}

/*Function declarations*/

void handler(int signumber){
  printf("[ PARENT ]: Signal with number %i has arrived\n",signumber);
}