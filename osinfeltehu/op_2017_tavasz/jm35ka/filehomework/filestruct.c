
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

struct Person {
  int year;
  char name[20];
};



int main(int argc, char** argv){

  struct Person person;
  int g;
  char y[10];
  
  strcpy(person.name, argv[1]);  
  person.year=atoi(argv[2]);
  sprintf(y, "%d", person.year);
  
  g=open(argv[3], O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
  
  if(g<0){perror("Error at opening the file\n"); exit(1);}
  
  if(write(g,&person.name, strlen(person.name))!=strlen(person.name))
  {perror("There is a mistake in writing\n");exit(1);}
  if(write(g,&y, strlen(y))!=strlen(y))
  {perror("There is a mistake in writing\n"); exit(1);}
    
  close(g);
  
  return 0;

}
