#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h>

#include <signal.h>
#include <sys/types.h>

void handler(int signumber)
{
    printf("Signal with number %i has arrived\n",signumber);
}
void president()
{
    pause();
    pause();


}
void checker()
{
    kill(getppid(),SIGTERM);
}
void sealer()
{
    kill(getppid(),SIGTERM);
}
int main()
{
    int status;
    int n;
    signal(SIGTERM,handler);
    printf("Enter N :");
scanf("%d",&n);
int num;
    int fd[2];
    pipe(fd);
    int pid = fork();
        /*stupid child*/

int fd[2];
    pipe(fd);
pid_t  child=fork(); //forks make a copy of variables
if (child<0)
{
    perror("The fork calling was not succesful\n");
    exit(1);
}
if (child>0) //the parent process, it can see the returning value of fork - the child variable!
{
    pid_t child2=fork();
    if (child2<0)
    {
        perror("The fork calling was not succesful\n");
        exit(1);
    }
    if (child2>0)
    {
        president();
        if((close(fd[0])) == -1){
          perror("close:");}
      int k;
      for (k=0;k<10;k++){
            int random=rand();
         write(fd[1], &random, sizeof(int));
      }
      close(fd[1]);

        waitpid(child2,&status,0);
        waitpid(child,&status,0); //wait(&status)
    }
    else
    {
        //second child
        sealer();
    }
}
else
{
    //1st child process
    checker();
    int i,k;
     if((close(fd[1])) == -1){
          perror("close:");}
     for (i=0;i<10;i++) {
        read(fd[0],&k,sizeof(int));
        printf("The random identification number in child is %d\n",k);
     }
     close(fd[0]);

}
return 0;
}
