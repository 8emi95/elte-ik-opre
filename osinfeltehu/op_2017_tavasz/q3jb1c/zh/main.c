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
    int pipefd[2]; // unnamed pipe file descriptor array -- PASSENGER -> CALLCENTER
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

    /* child process -- Passenger */
    if (0 == child1)    
    {
           printf("[ CHILD1 ]: I am the FIRST child. My PID: %d, and my parnet's PID: %d\n", getpid(), getppid());
           
           /*Send signal to get ready*/
            printf("[ CHILD ]: Logged in succesfully! \n");
            kill(getppid(), SIGTERM); 

           /*Here DO what CHILD1 needs to do!! */
           close(pipefd[0]);
           write(pipefd[1], &argv[1], sizeof(argv[1]));
           printf("[ CHILD1 ]: Sent the adress: %s\n", argv[1]);

           printf("[ CHILD1 ]: FINISHED\n");
    
    }
    else{
        //TAXI
        child2 = fork();
        if (child2 == -1) {
            perror("Error: Fork -- CHILD2\n");
            exit(EXIT_FAILURE);
        }
        if(child2 == 0){
            printf("[ CHILD2 ]: I am the SECOND child. My PID: %d, and my parent's PID: %d\n", getpid(), getppid());
            
            /*Here DO what CHILD2 needs to do!! */
            //pause();
            
            printf("[ CHILD2 ]: FINISHED\n");
        }
        //PARENT
        if(child2 != 0){ 
            printf("[ PARENT ]: This is the parent process!\n");
            printf("[ PARENT ]: I am the parent my pid is %d\n", getpid());
            
            //Waiting for the signal
            printf("[ PARENT ]: Waiting for the login of the child!\n");
            //sleep(1);
            //kill(child1, SIGTERM);
            pause(); //waits till a signal arrive 
            printf("[ PARENT ]: Signal arrived\n",SIGTERM);
            
            /*Here DO what PARENT needs to do!! */
            close(pipefd[1]);
            read(pipefd[0], &argv[1], sizeof(argv[1]));
            printf("[ PARENT ]: The given address is : %s\n", argv[1]);
        
            //wait(&status);
            
            waitpid(child1, &status, 0);
            waitpid(child2, &status, 0);
            
           // kill(child2, SIGTERM);
            printf("[ PARENT ]: Parent has finished!\n");
        }
    }
    return 0;
}
/*Function declarations*/

void handler(int signumber){
  printf("[ PARENT ]: Signal with number %i has arrived\n",signumber);
}