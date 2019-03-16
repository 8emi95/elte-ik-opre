#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
void kuld(int f)
{
 char uzenet[80];
 printf(">");scanf("%s",uzenet);
 write(f,uzenet,strlen(uzenet)+1);
}
void kap(int f)
{
  char valasz[80];
  read(f,valasz,80);
  printf("%s\n",valasz);fflush(stdout);
}
int main(int argc,char* argv[] )
{
   char valasz[80],uzenet[80];
   int f1,f2;
   f1=open("cso",O_RDONLY);
   f2=open("cso2",O_WRONLY);
   while (1)
   {
     kap(f1);
     kuld(f2);
   }
}