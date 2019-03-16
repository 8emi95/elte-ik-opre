#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   //fork
#include <sys/wait.h> //waitpid
#include <errno.h>

int main() {
/*
    int i;
    for (i = 1; i < 4; ++i) {
        fork();
    }
    printf("Hello %i %i\n", getpid(), getppid());
*/    

//Szülő és 2 gyerek folyamat
    int status;
    int parent_pid = getpid();
    pid_t child = fork();
    if(getpid() == parent_pid) {
        pid_t child2 = fork();
        waitpid(child, &status, 0);
        waitpid(child2, &status, 0);
    }
    printf("Hello %i %i\n", getpid(), getppid());

//2. megoldás
/*
    int status;
    int notacommonvalue = 1;
    printf("The value is %i before forking \n", notacommonvalue);

    pid_t child = fork(); 
    if (child > 0)
    {
        fork();
        waitpid(child, &status, 0);
        wait(&status, 0);
        printf("The value is %i in parent process (remain the original) \n", notacommonvalue);
    }
*/
    return 0;
}