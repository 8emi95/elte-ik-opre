#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#
#include <pwd.h>
int main(){
 /*DIR *ActDir;
 ActDir=opendir(".");
 struct dirent *ActF;
 struct stat st;
 while (ActF=readdir(ActDir))
 {
    printf("%s\n",ActF->d_name);
    stat(ActF->d_name,&st);
    printf("%s\n",getpwuid(st.st_uid)->pw_name);
 }
 */
// if (mkdir("konyvtar",S_IRWXU|S_IRWXG)){perror("hiba");};
 
 DIR *d;
 long poz;
 struct dirent *de;
 d=opendir(".");
 de=readdir(d);
 printf("%s\n",de->d_name);
 de=readdir(d);
 printf("%s\n",de->d_name);
 poz=telldir(d);
 de=readdir(d);
 printf("%s\n",de->d_name);
 seekdir(d,poz);
 de=readdir(d);
 printf("%s\n",de->d_name);
  
return 0;
}