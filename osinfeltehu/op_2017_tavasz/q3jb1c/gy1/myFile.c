#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

struct MyStruct
{
  char array[20];
  char len;
};

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    printf("Wrong number of parameters.");
    return 1;
  }
  int f;
  f = open(argv[1], O_WRONLY |O_RDONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  struct MyStruct c;
  c.len = '2';
  //c.array = (char*)malloc(10*sizeof(char));
  strcpy(c.array, "Gipsz Jakab");
  write(f, &c, sizeof(c));
  close(f);
  f = open(argv[1], O_RDONLY);
  struct MyStruct other;
  read(f,&other,sizeof(other));
  printf("Str: %s\n", other.array);
  printf("Len: %d\n", other.len);
  close(f);
  return 0;
}