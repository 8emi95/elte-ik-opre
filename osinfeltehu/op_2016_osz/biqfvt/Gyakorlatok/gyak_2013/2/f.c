#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <sys/types.h> //waitpid
#include <errno.h> //perror 
#include <string.h>
char sz[]="Hello";
int j=5;
int main()
{
 printf("%i\n",getpid()); 
 int i=5;
 usleep(300);
 pid_t ch=fork();//fork makes a copy of variables
 if (ch>0){
   usleep(200);
   pid_t ch2=fork();
   if (ch2>0){
     usleep(200);
   }else {usleep(200);}
 } else {usleep(200);}
 printf("Hello long text text text text %i %i\n",getpid(),getppid());
return 0;
}
