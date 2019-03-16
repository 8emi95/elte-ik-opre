#include <stdlib.h>
#include <stdio.h>
#include <errno.h> //hibakezelhez kell

int main(int argc, char * argv[])
{ 
  FILE * f;
  char fnev[]="adat";
  char sor[80];
  f=fopen(fnev,"r");
  if (errno){
   perror("Hiba -");
   exit(0);
  };
  while (fgets(sor,80,f))
  {
    printf("%s",sor);
  };
  fclose(f);
  return 0;
}