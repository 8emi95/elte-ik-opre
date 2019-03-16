#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>


#include "2Str.h"

int main(int argc, char *argv[]){
  //initialization
  struct dataS data1;
  char output[104];
  char buffer[10];
  int file;
  
  printf("Add name then year of birth\n");
  //Scanning
  scanf("%s",data1.name);
  scanf("%d",&data1.year);
  //From input to char array
  strcat(output,data1.name);
  sprintf(buffer, "%d", data1.year);
  size_t length = strlen(output);
  output[length] = ' ';
  output[length+1] = '\0';
  strcat(output,buffer);
  
  // File I/O
  file = open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
  if(file<0) {
    perror("Error creating file \n");
    exit(1);
  }
  int i = 0;
  while(output[i] != '\0') {
    write(file,&output[i],sizeof(output[i]));
    i++;
  }
  printf("%s \n",output);
  printf("Name: %s Year of birth: %d \n",data1.name,data1.year);
  return 0;
}