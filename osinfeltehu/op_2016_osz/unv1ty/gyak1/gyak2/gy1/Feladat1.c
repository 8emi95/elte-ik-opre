//csináljunk 1 szülõt 2 gyereket
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 

int main()
{
   printf( "szulo pid %i\n", getpid() );

    for ( int i = 0; i < 3; i++ ) //ellenõrizzük szülõben vagyunk-e
        if ( fork() == 0 )
        {
            printf( "gyerek pid %i from pid %i\n", getpid(), getppid() );
            exit( 0 );
        }

    for (int i = 0; i < 3; i++)
        wait(NULL);
}

/*
int status;

int parent_pid = getpid();
pid_t child = fork();
if(getpid()==parent_pid)
{
  pid_t child2 = fork();
  waitpid(child, &status, 0);
  waitpid(child2, &status, 0);
}
printf("Hello %i %i\n", getpid(), getppid());
return 0;
*/

/*
int status;
 int notacommonvalue=1;
 printf("The value is %i before forking \n",notacommonvalue);
 
 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0) //the parent process, it can see the returning value of fork - the child variable!
 {
    fork(); //ide a szülõbe írunk egy fork()-ot, és akkor megint 2 gyerek van
    waitpid(child,&status,0); 
    //waits the end of child process PID number=child, the returning value will be in status
    //0 means, it really waits for the end of child process - the same as wait(&status)
    printf("The value is %i in parent process (remain the original) \n",notacommonvalue);
 
 }
 else //child process
 {
    notacommonvalue=5; //it changes the value of the copy of the variable
    printf("The value is %i in child process \n",notacommonvalue);
 }
 return 0;

*/