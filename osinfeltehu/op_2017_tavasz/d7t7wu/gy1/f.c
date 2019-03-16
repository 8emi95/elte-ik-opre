#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h>


int main()
{
 int i;
 //for (i = 0; i < 5; i++)
 fork(); //forks make a copy of variables
 printf("Ez:");
 return 0;
}
