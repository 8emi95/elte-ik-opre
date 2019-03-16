#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define BUFFERSIZE 4096
#define MAX_LENGHT 1024

int file_exist (char *filename) {
  struct stat buffer;
  return (stat (filename, &buffer) == 0);
}

void read_from_file(char filename[]){
  char output[BUFFERSIZE];
  int f;
  f=open(filename,O_RDONLY);
  if(f<0){
    perror("Error opening file \n");
    exit(1);  
  }
  size_t nbytes;
  ssize_t bytes_read;
  nbytes = sizeof(output);
  bytes_read = read(f,output,nbytes);
  output[bytes_read] = '\0';
  printf("%s\n",output);
  close(f);
}

void append_to_file(char filename[], char data[]) {
  int f;
  mode_t mode = S_IRUSR | S_IWUSR;
  char buffer[BUFFERSIZE];
  //char *buffer = malloc (BUFFERSIZE);
  f = open(filename,O_RDWR | O_APPEND,mode); 
  if(f<0) {
    perror("Error opening file \n");
    exit(1);
  }
  
  //size_t length = strlen(data);
  strcpy(buffer,data);
  //char bn[] = '\n';
  //strcat(buffer,"\n");
  char c;
  int i = 0;
  size_t n = strlen(buffer);
  if(write(f,&buffer,n)<0) {
    perror("Error writing file \n");
    exit(1);
  }
  close(f);
}

void create_file(char filename[], char data[]) {
  int f;
  char yn[BUFFERSIZE];
  char yes[] = "yes";
  char no[] = "no";
  int bool = 1;
  mode_t mode = S_IRUSR | S_IWUSR;
  if(file_exist(filename)) {
    printf("FILE EXISTS!\n");
    printf("Are you sure you would like to overwrite this file?\n");
    //fgets(yn,BUFFERSIZE,stdin);
	size_t n = strlen(yn);
	yn[n-1] = '\0';
	while(bool) {
	  fgets(yn,BUFFERSIZE,stdin);
	  size_t n = strlen(yn);
	  yn[n-1] = '\0';
	  if(strncmp(yn,yes,strlen(yn)) == 0) {
	    bool = 0;
	  }
	  else if(strncmp(yn,no,strlen(yn)) == 0) {
	    printf("Going back the menu.\n");
		exit(0);
	  }
	}
  }
  char buffer[BUFFERSIZE];  
  f = open(filename,O_RDWR|O_CREAT|O_TRUNC,mode);
  if(f<0) {
    perror("Error creating file \n");
    exit(1);
  }
  strcpy(buffer,data);
  size_t n = strlen(buffer);
  if(write(f,&buffer,n)<0) {
    perror("Error writing file \n");
    exit(1);
  }
  close(f);
}




int main(int argc, char *argv[]) {
  char filename[BUFFERSIZE];
  char input[BUFFERSIZE];
  //scanf("%s",filename);
  //fgets(filename,BUFFERSIZE, stdin);
  //printf("----------\n");
  fgets(input,BUFFERSIZE, stdin);
  //size_t n = strlen(input);
  //input[n] = '\n';
  //scanf("%s",input);
  //append_to_file(argv[1],input);
  create_file(argv[1],input);
  read_from_file(argv[1]);
  return 0;
}