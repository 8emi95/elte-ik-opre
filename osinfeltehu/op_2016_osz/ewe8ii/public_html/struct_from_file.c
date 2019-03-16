//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h> 

struct Student {
  int grade;
  char  name[50];
}; 


int main(int argc,char** argv){ 


  struct Student stud;
 
 if (argc!=2){perror("You have to use program with two arguments, the file names copy_from copy_to");exit(1);}
 int f;
 f=open(argv[1],O_RDONLY); 
 
 read(f,&stud,sizeof(stud));

 printf("%s\n", stud.name);
  printf("%i\n", stud.grade);
 
 close(f);
 return 0;
}