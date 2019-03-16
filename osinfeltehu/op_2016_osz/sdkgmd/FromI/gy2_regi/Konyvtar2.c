#include <stdlib.h>
#include <stdio.h>
#include <dirent.h> //readdir
#include <sys/types.h>//readdir

int main()
{  DIR* d; 
 struct dirent *direntp;

  printf("Az aktualis konyvtar tartalma: \n");
  d=opendir(".");
  while ((direntp=readdir(d))!=NULL)
   { 
     printf("%s\n",direntp->d_name);
   }
  closedir(d);

  return 0;
}