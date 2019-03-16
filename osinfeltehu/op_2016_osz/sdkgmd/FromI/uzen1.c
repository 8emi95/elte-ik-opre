#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void kuld(int f)
{    char uzenet[80];
     printf(">");   scanf("%s",uzenet);
     write(f,uzenet,strlen(uzenet)+1);
}
void kap(int f)
{   char valasz[80];
    read(f,valasz,80);
    printf("%s\n",valasz);fflush(stdout);
 
}
int main(int argc, char * argv[])
{
   int status,f1,f2;
   status=mkfifo("cso",S_IRUSR | S_IWUSR);
   if (status<0){printf("Hibas");exit(1);}
   status=mkfifo("cso2",S_IRUSR | S_IWUSR);
   if (status<0){printf("Hibas cso2");exit(1);}
   f1=open("cso",O_WRONLY);
   f2=open("cso2",O_RDONLY);
   while (1)
  {  kuld(f1);
     kap(f2);
   } 
}