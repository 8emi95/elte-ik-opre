#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 int i;
 for (i=1;i<4;i++)
  fork();
 fork();
 // getpid() az a getprocessid
 // a get ppid meg a get parent process id. ha 1-essel tér vissza, akkor a szülő korábban befejeződött,
 // minthogy lekértük az volna az idjét 
 printf("Hello myid:%i parent: %i \n",getpid(), getppid());
 return 0;
}