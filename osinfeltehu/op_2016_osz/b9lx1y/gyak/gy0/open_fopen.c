//Read and print out the file given in the parameter 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h> //perror, errno
#include <string.h>


struct Record {
 char name[60];
 int mark;
};


void use_open_bin(char* fname){
 printf("\n**********\nUsing open - binary \n****************\n");
 int f=open(fname,O_RDONLY); 
 if (f<0){ perror("Error at opening the file\n");exit(1);}
 
 char c; //instead of char, you can use any other type, struct as well
 while (read(f,&c,sizeof(c))){ //use write for writing
    printf("%c",c); //we prints out the content of the file on the screen
 } 
 printf("\n"); 
 lseek(f,0,SEEK_SET); //position to the first character, SEEK_SET,SEEK_CUR,SEEK_END
 read(f,&c,sizeof(c));
 printf("First character - after positioning %c\n",c);
 close(f);
}

void use_fopen_text(char* fname){
 printf("\n******************\nUsing fopen -  reads in lines\n*****************\n");
 FILE * f;
 f=fopen(fname,"r");
 if (f==NULL){perror("File opening error\n"); exit(1);}
 char line[160];
 while (!feof(f)){
   fgets(line,sizeof(line),f);
   printf("%s",line);
 } 
 printf("\n");
 fclose(f);
}

void use_fopen_bin(char* fname){
 printf("\n******************\nUsing fopen -  binary \n*****************\n");
 FILE * f;
 f=fopen(fname,"rb");
 if (f==NULL){perror("File opening error\n"); exit(1);}
 char c; //instead of char, you can use any other type, struct as well
 while (!feof(f)){
   fread(&c,sizeof(c),sizeof(c),f); //use fwrite for writing
   printf("%c",c);
 } 
 printf("\n");
 fseek(f,0,SEEK_SET); //position to the first character, SEEK_SET,SEEK_CUR,SEEK_END
 fread(&c,sizeof(c),sizeof(c),f);
 printf("First character - after positioning %c\n",c);
 
 fclose(f);
}

int main(int argc,char** argv){
 char name[] = "Gabor";
 int mark = 5;
 struct Record rec;
 strcpy(rec.name, name);
 rec.mark = mark;

 char file_name[] = "serialized_rec.bin";

 if (open(file_name, O_CREAT | O_WRONLY, 0777) < 0) {
  perror("open");
  return 1;
 }

 if (write(file, &rec, sizeof(rec)) < 0) {
  perror("write");
  return 1;
 }

 if (close(file) < 0) {
  perror("close");
 }

 return 0;
}
