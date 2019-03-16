//print out the last modification of file


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h> //stat
#include <unistd.h>   //stat
#include <time.h> //ctime

int main(int argc,char** argv){
 struct stat st;
 stat(argv[0],&st);  //fstat(file descriptor, stat structure)
 time_t t=st.st_mtime;
 printf("The last modification was: %s\n\n",ctime(&t)); //convert time_t to string
 time_t now = time(0); // Get the system time
 printf("most: %s\n",ctime(&now));
 return 0;
}
