#include <stdlib.h>  
#include <stdio.h>
#include <sys/types.h> //fork-hoz
#include <sys/stat.h>
#include <unistd.h> //fork
#include <string.h> //strlen
#include <fcntl.h> //lock
int main()
{
  pid_t child;
  child = fork();
  if (child<0){ //hiba
    perror("Error"); exit(1);
  }
  if(child==0)
  {
    printf("gyerek");
  }
  else
  {
      printf("szulo");
      int status;
      waitpid(child,&status,0);
  }
  return 0;
}
