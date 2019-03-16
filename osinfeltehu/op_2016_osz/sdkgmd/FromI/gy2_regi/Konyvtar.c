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
 char New_form_s[80];
 char fname[80];
 strcpy(New_form,form);
 strcat(New_form,"\t");    //kovetkezo szinthez
 strcpy(New_form_s,form);
 strcat(New_form_s,"%s\n"); //aktualis filnev kiirasahoz

 chdir(di);
 d=opendir(".");
 if (!errno)
 {
   while ((direntp=readdir(d))!=NULL)
   { 
     strcpy(fname,direntp->d_name);
     printf(New_form_s,fname);
     if (((strcmp(fname,".")!=0) && ((strcmp(fname,"..")!=0))))
     {
      stat(fname,&My_status);
      if (S_ISDIR(My_status.st_mode)){ //ha konyvtar
        strcat(New_form,"\t");
        Konyvt(New_form,fname);
      }
     };
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