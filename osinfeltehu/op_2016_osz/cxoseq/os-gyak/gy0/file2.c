//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>

int main(int argc,char** argv){

 struct person {
  char name[25];
  int grade;
 } p;

 strcpy(p.name,"John Smith");
 p.grade = 4;
 printf("%s", p.name);

 int f;
 f=open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR); 

 if (f<0){ perror("Error at opening the file\n");exit(1);}
 if (write(f,&p,sizeof(p)) != sizeof(p)) {
  perror("Failed to write data into file!");
  exit(1);
 }

 close(f);

 return 0;
}
