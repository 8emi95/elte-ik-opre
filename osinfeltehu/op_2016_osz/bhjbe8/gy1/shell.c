#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
  char in[10];
  printf("Parancs: ");
  scanf("%s", &in);
  printf("\n\n%s", in);
 return 0;
}