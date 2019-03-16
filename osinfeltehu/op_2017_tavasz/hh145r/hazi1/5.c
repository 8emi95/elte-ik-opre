#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc,char** argv){

 FILE * f;
 f=fopen("5.txt","w");
 if (f==NULL){
  perror("File creating error\n");
  exit(1);
 }
 if(fputs("Buzogány Attila",f)<0){
  perror("File writing error\n");
  exit(1);
 } 
 fclose(f);

 return 0;
}