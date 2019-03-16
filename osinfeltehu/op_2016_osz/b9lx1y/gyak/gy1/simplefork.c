#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h>

int main()
{
 int status;
 pid_t child=fork(); //forks make a copy of variables

 if (child > 0) {
   printf("Szulo");
   waitpid(child, &status, 0);
 }

 if (child == 0) {
   printf("Gyerek");
 }

 return 0;
}
