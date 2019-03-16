#include <stdlib.h>
#include <stdio.h>
#include <errno.h> //hibakezelhez kell
#include <fcntl.h>
#include <sys/stat.h>
int main(int argc, char * argv[])
{ 
  int f,g;
  int szam; // text fajlba fura ertekek - nem ASCII!!!
  if (access("input",X_OK)==0){printf("VAn");}else {printf("Nincs");exit(0);};
  f=open("input",O_RDONLY); //olvasasra nyit meg
   if (errno){
   perror("Hiba -");
   exit(0);
  };
 g=open("output",O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
  if (errno){
   perror("Hiba -");
   exit(0);
  };
  read(f,&szam,sizeof(int));
  printf("%i\n",szam);
  write(g,&szam,sizeof(int));
  close(g);
  close(f);
  return 0;
}