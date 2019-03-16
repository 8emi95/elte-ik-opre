#include <stdlib.h>
#include <stdio.h>
#include <errno.h> //hibakezelhez kell
#include <dirent.h> //readdir
#include <sys/types.h>//readdir
#include <string.h>
#include <sys/stat.h> //S_ISDIR es stat tipus

void Konyvt(char form[],char di[]){
 DIR* d; 
 struct dirent *direntp;
 struct stat My_status;
 char New_form[80];
 
 strcpy(New_form,form);
 strcat(New_form,"\t");    //kovetkezo szinthez

 chdir(di);
 d=opendir(".");
 if (!errno)
 { 
   while ((direntp=readdir(d))!=NULL)
   { 
     if ((strcmp(direntp->d_name,".")!=0) && (strcmp(direntp->d_name,"..")!=0)){ 
           printf(form);
     printf("%s\n",direntp->d_name);

      stat(direntp->d_name,&My_status);
      if (S_ISDIR(My_status.st_mode)){ //ha konyvtar
        Konyvt(New_form,direntp->d_name);
      }
   }
   }
  };
  closedir(d);
  chdir(".."); 
}
int main()
{ char d[80]=".";
  printf("Az aktualis konyvtar tartalma: \n");
  Konyvt(" ",d);
  return 0;
}