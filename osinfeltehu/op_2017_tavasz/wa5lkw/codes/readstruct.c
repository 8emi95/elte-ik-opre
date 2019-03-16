//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <unistd.h>
#include <string.h>
int main(int argc,char** argv){
 int f;
 
 f=open("struct.dat",O_RDONLY); 
 if (f<0){ perror("Error at opening the file\n");exit(1);}
 
 struct passenger{
   char name[50];
   char phone[10];
   char town[10];
 } pass;

 read(f,&pass,sizeof(pass));
 int l;
 l=strlen(pass.name); pass.name[l-1]=0;
 l=strlen(pass.phone); pass.phone[l-1]=0;
 l=strlen(pass.town); pass.town[l-1]=0;  
 close(f);
 printf("%s, %s, %s\n",pass.name,pass.phone,pass.town);
 return 0;
}
