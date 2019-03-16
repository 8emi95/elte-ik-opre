/*
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h>


int main()
{
 int status;
 int notacommonvalue=1;
 printf("The value is %i before forking \n",notacommonvalue);

 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);}
 if (child>0) //the parent process, it can see the returning value of fork - the child variable!
 {
    waitpid(child,&status,0);
    //waits the end of child process PID number=child, the returning value will be in status
    //0 means, it really waits for the end of child process - the same as wait(&status)
    printf("The value is %i in parent process (remain the original) \n",notacommonvalue);
 printf("child erteke: %i in parent process\n",getpid());
 }
 else //child process
 {
    notacommonvalue=5; //it changes the value of the copy of the variable
    printf("The value is %i in child process \n",notacommonvalue);
    printf("child erteke: %i in child process\n",getpid());
 }
 return 0;
}
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    printf("--beginning of program\n");

    pid_t child_pid, wpid;
    int status = 0;
    int i;

    int counter = 0;
    pid_t pid ;

for (i = 0; i < 3; i++)
    {
    if ((pid=fork()) == 0)
    {
        if (i == 0){
            // child process
             printf(" in child0 process: pid0=%d\n", getpid());
            int j = 0;
            for (; j < 5; ++j)
            {
                printf("child0 process: counter=%d\n", ++counter);
            }
                printf("--end  of child%i program--\n",i);
            exit(1);
        }
        if (i == 1){
            // child process
            printf(" in child1 process: pid1=%d\n", getpid());
            int j = 0;
            for (; j < 7; ++j)
            {
                printf("child1 process: counter=%d\n", ++counter);
            }
                printf("--end  of child%i program--\n",i);
            exit(1);
        }
        if (i == 2){
            // child process
            printf(" in child2 process: pid2=%d\n", getpid());
            int j = 0;
            for (; j < 9; ++j)
            {
                printf("child2 process: counter=%d\n", ++counter);
            }
                printf("--end  of child%i program--\n",i);
            exit(1);
        }
    }
 /*   else
    {
        // fork failed
        printf("fork() failed!\n");
        return 1;
    }
*/

    }
        // parent process
        printf(" in parent process: pidp=%d\n", getpid());
    while ((wpid = wait(&status)) > 0)
    {
        printf("Exit status of %d was %d (%s)\n", (int)wpid, status,
               (status > 0) ? "accept" : "reject");
    }
        //exit(1);
        printf("--end  of parent program--\n");
    return 0;
}
