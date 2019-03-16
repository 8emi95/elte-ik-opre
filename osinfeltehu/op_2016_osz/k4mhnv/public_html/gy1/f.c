#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 

int main()
{
    pid_t child = fork(); 

    

    if(child > 0){
        int status;

        fork();

        waitpid(child,&status,0);
    }

printf("Hello %i %i\n", getpid(), getppid());

    return 0;
}