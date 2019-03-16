//Read and print out the file given in the parameter 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h> //perror, errno
#include <string.h>


struct Student {
 char name[25];
 int mark;
};

int main(int argc,char** argv){
 char name[] = "Gabor";
 int mark = 5;

 struct Student rec;

 strcpy(rec.name, name);
 rec.mark = mark;

 char file_name[] = "serialized_rec.bin";

 int fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
 if (fd < 0) {
  perror("open");
  return 1;
 }
 
 int written = write(fd, &rec, sizeof(rec));
 if (written != sizeof(rec)) {
  perror("write");
  return 1;
 }

 if (close(fd) < 0) {
  perror("close");
 }

 return 0;
}
