#include <stdlib.h>
#include <stdio.h>
#include <errno.h> //hibakezelhez kell
#include <fcntl.h>
#include <sys/stat.h>
int main(int argc, char * argv[])
{ 
  int f;
  char fnev[]="adat";
  char sor[80];
  f=open(fnev,O_RDONLY); //olvasasra nyit meg
//  f=open("uj",O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
  if (errno){
   perror("Hiba -");
   exit(0);
  };
  while (read(f,sor,80))
  {
    printf("%s",sor);
  };
  printf("\n");
  close(f);
  return 0;
}