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
 
 int g; 
 g=open("struct.dat",O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
 if (g<0){ perror("Error at opening the file\n");exit(1);}
 
 struct passanger{
    char name[50];
    char phone[10];
    char town[10];
 } pass;
 
 char line[50];
 fgets(line,50,stdin);
 strcpy(pass.name,line);
 fgets(line,10,stdin);
 strcpy(pass.phone,line);
 fgets(line,10,stdin);
 strcpy(pass.town,line); 
  
 write(g,&pass,sizeof(pass)); 
 
 close(g);
 return 0;
}