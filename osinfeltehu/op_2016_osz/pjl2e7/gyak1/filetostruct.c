#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "datastruct.h"


int main(int argc, char** argv){
  if(argc != 2){perror("This program expects exactly one parameter!\n");exit(1); 
  }
  int f;
  
  if(access(argv[1], F_OK) < 0){perror("Could'nt access the file!\n");exit(1);}
  
  f = open(argv[1], O_RDONLY);
  
  if(errno != 0 || f < 0){perror("Could'nt open the file!\n"); exit(1);}
  
  struct DataStruct d;
  
  if(read(f, &d, sizeof(DataStruct)) != sizeof(DataStruct)){
    perror("Error while reading!\n"); exit(1);
  }else{
    printf("%s, %i", d.name, d.grade);
  }

}