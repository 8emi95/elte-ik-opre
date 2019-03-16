//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>

struct Structure
{
  int number;
  char *array;
};

int main(int argc,char** argv){
 
  if (argc!=2){perror("You have to use program with two arguments, the file names copy_from copy_to");exit(1);}
  int f,g;
 
  g=open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
  if (g<0){ perror("Error at opening the file\n");exit(1);}
 
  struct Structure s;
  s.number = 100;
  s.array = (char*)malloc(200*sizeof(char));
  strcpy(s.array,"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
//  printf("\n%i\n", sizeof(s.array));

  write(g,&s,sizeof(s));
  close(g);

  f=open(argv[1],O_RDONLY);
  if (f<0){ perror("Error at opening the file\n");exit(1);}

  struct Structure ss;
  read(f,&ss,sizeof(s));

  close(f);

  printf("%i\n",ss.number);
  printf("%s\n",ss.array);

  return 0;
}