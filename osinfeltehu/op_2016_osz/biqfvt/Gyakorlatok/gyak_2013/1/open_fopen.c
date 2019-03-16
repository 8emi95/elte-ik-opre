//Read and print out the file given in the parameter 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>//open
#include <errno.h> //perror, errno
#include <string.h>


void use_open_bin(char* fname){
 printf("\n**********\nUsing open - binary \n****************\n");
 int f=open(fname,O_RDONLY); //if the opening is not successful, the f equals -1
 //if (f<0){ perror("Error at opening the file \n");exit(1);}
 if (errno!=0) {perror("Error at opening the file \n");exit(1);}
 //if errno > 0 then there is some problem - errno contains the error number 
 char c; //instead of char, you can use any other type, struct as well
 while (read(f,&c,sizeof(c))){ //use write for writing
    printf("%c",c); //we prints out the content of the file on the screen
 } 
 printf("\n"); 
 lseek(f,0,SEEK_SET); 
 //1. parameter - file descriptor
 //2. parameter - the relative possition of file pointer
 //3. parameter - the absolute position of file pointer
 //position of the first character, SEEK_SET (very start of file),SEEK_CUR (current),SEEK_END
 // returns back the position of the file pointer in bytes
 // size of a structured bin file =>  lseek(f,0,SEEK_END)/sizeof(structure)  
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
   fgets(line,sizeof(line),f); //fgets reads in the end of line character as well!!! 
                               //fgets(variable,maxlength,source); source can be stdin!!
                               //scanf(formatum string, address of variable) - e.g scanf("%i",i)
                               //scanf lets the end of line character on the buffer
   printf("%s,",line);
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
 fseek(f,0,SEEK_SET); 
 //1. parameter - file descriptor
 //2. parameter - relative position
 //3. parameter - absolute position
 //position of the first byte, SEEK_SET,SEEK_CUR,SEEK_END
 fread(&c,sizeof(c),sizeof(c),f);
 printf("First character - after positioning %c\n",c);
 
 fclose(f);
}
int main(int argc,char** argv){
 if (argc!=2) 
 {perror("Give a filename as a command line argument\n");exit(1);}
 if (!access(argv[1],F_OK)==0){perror("The file is not exist!\n"); exit(1);}
 //F_OK = the files exists, R_OK = we have read permission, 
 //W_OK = we have write permisssion, X_OK =we have permission of execution
 use_fopen_text(argv[1]);
 use_open_bin(argv[1]);
 use_fopen_bin(argv[1]);
 return 0;
}