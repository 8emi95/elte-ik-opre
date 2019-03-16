#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

struct DataStruct{
  char name[50];
  int grade;
};

int main(int argc, char** argv){
  struct DataStruct data1;
  
  strcpy(data1.name, "Gipsz Jakab");
  data1.grade = 5;

  if(argc != 2){perror("This program needs exactly one argument!");exit(1);}

  int f;
  
  f = creat(argv[1], S_IRUSR|S_IWUSR);
  
  if(f < 0 || errno != 0){perror("Could'nt create file!");exit(1);}

  if(write(f, &data1, sizeof(data1)) != sizeof(struct DataStruct)){
    perror("Could'nt write the file!\n");exit(1);
  } 
  

}