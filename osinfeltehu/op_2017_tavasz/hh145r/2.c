#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>

void use_fopen_text(char* fname){
 printf("\n******************\nUsing fopen -  reads in lines\n*****************\n");
 FILE * f;
 f=fopen(fname,"r");
 if (f==NULL){perror("File opening error\n"); exit(1);}
 char line[160];
 while (!feof(f)){
   if(fgets(line,sizeof(line),f)){
   printf("%s",line);
}
 } 
 printf("\n");
 fclose(f);
}

int main(int argc,char** argv){
 if (argc!=2) {perror("Give a filename as a command line argument\n");exit(1);}
 if (!access(argv[1],F_OK)==0){perror("The file is not exist!\n"); exit(1);}
 use_fopen_text(argv[1]);


 return 0;
}
