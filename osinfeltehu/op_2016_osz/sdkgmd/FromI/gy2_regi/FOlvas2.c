#include <stdlib.h>
#include <stdio.h>
#include <errno.h> //hibakezelhez kell
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char * argv[])
{ 
  int f,f2;
  char fnev[]="adat";
  char fnev2[]="masolat";
  char sor[80]="                                                      ";
  //ha van kezdoerteke, akkor helyesen mukodik - itt a sorvege jel...
  f=open(fnev,O_RDONLY,0);
  f2=creat(fnev2,S_IREAD|S_IWRITE);
  
  f2=open(fnev2,O_WRONLY,0);
  
  if (errno){
   perror("Hiba -");
   exit(0);
  };

  while (read(f,sor,80)>0)
  { //problema a file vegen a sorban maradt szemetet kiirja 
    printf("%s",sor);
    if (strlen(sor)!=0){
       printf("sorhossz:%i\n",strlen(sor));
       if (write(f2,sor,80)==0){perror("Hiba");};
    }  

  };

  close(f);
  close(f2);
  return 0;
}