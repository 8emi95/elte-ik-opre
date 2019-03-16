#include <stdlib.h>
#include <stdio.h>
#include <errno.h> //hibakezelhez kell
#include <dirent.h> //readdir
#include <sys/types.h>//readdir
#include <string.h>
#include <sys/stat.h> //S_ISDIR es stat tipus
#include <time.h> //ctime
#include <pwd.h> //user nev kiolvasasa
#include <grp.h> //group id-hoz
char* uname(uid_t user)
{
  char * usern=(char*)malloc(25*sizeof(char));
  struct passwd * ps=getpwuid(user);
  strcpy(usern,ps->pw_name);
  return usern;
}
char* gname(gid_t usergroup)
{
  char * g_name=(char*)malloc(25*sizeof(char));
  struct group * g=getgrgid(usergroup);
  strcpy(g_name,g->gr_name);
  return g_name;
}
char * atr(int a)
{
  char* atrb=(char*)malloc(12*sizeof(char));
  if (a & S_IFDIR){strcpy(atrb,"d");}else {strcpy(atrb,"-");}
  if (a & S_IRUSR){strcat(atrb,"r");}else {strcat(atrb,"-");}
  if (a & S_IWUSR){strcat(atrb,"w");}else {strcat(atrb,"-");}
  if (a & S_IXUSR){strcat(atrb,"x");}else {strcat(atrb,"-");}
  if (a & S_IRGRP){strcat(atrb,"r");}else {strcat(atrb,"-");}
  if (a & S_IWGRP){strcat(atrb,"w");}else {strcat(atrb,"-");}
  if (a & S_IXGRP){strcat(atrb,"x");}else {strcat(atrb,"-");}
  if (a & S_IROTH){strcat(atrb,"r");}else {strcat(atrb,"-");}
  if (a & S_IWOTH){strcat(atrb,"w");}else {strcat(atrb,"-");}
  if (a & S_IXOTH){strcat(atrb,"x");}else {strcat(atrb,"-");}
  return atrb;
  
}
void lsal()
{
 DIR* d; 
 struct dirent *direntp;
 struct stat My_status;
 char fname[80];
 uid_t user; //getent passwd|grep  -pel megkapom a nevet
 gid_t usergroup;
 int attrib;
 time_t lastm_time;
 off_t meret;
 
 d=opendir(".");
 if (!errno)
 {
   while ((direntp=readdir(d))!=NULL)
   { 
     strcpy(fname,direntp->d_name);
     stat(fname,&My_status);

     lastm_time=My_status.st_mtime;
     user=My_status.st_uid;
     usergroup=My_status.st_gid;
     meret=My_status.st_size;
     attrib=My_status.st_mode;
     char dat[80]="";
     strncat(dat,ctime(&lastm_time),strlen(ctime(&lastm_time))-1); //strncat atmasolja az elso n byteot 
       //ctime stringet ad vissza - sorvege jelet letorlom
     printf("%s\t",atr(attrib));
     printf("%s",uname(user));
     printf(" %s \t",gname(usergroup));
     printf("%i\t %s \t%s\n",(int)meret,dat,fname);
   };
  };
  closedir(d); 
}
int main()
{ char nev[80];
  printf("Az aktualis konyvtar tartalma: \n");
  printf("Attrib\t\tUser Usergroup \t Bytes\t \tDate\t\t\tFile\n\n");
  lsal();
  return 0;
}