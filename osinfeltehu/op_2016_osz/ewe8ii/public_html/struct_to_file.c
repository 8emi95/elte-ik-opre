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
  stud.grade = 5;
  strcpy(stud.name,"Example");
  
 
 if (argc!=2){perror("You have to use program with two arguments, the file names copy_from copy_to");exit(1);}
 int g;
 
 
 g=open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);

 if (g<0){ perror("Error at opening the file\n");exit(1);}

write(g,&stud,sizeof(stud));

 
 close(g);
 return 0;
}