#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main(void){

  FILE *pFile;

  char * buffer;
  
  size_t dataInFile;
  
  long fileSize;
  
  pFile = fopen("names.bin", "rb+");
  //b for binary;
  
  if(pFile==NULL){
    perror ("Error Occured");
    printf ("Error Code: %d\n", errno);

    printf("File bieng created \n\n");
  pFile = fopen("names.bin", "wb+");
    
     if(pFile==NULL){
       perror ("Error Occured");
       printf ("Error Code: %d\n", errno);
     
       exit(1);
     }  
  }
  
  char name[] = "Marouane Blej";
    fwrite(name, sizeof(name[0]), sizeof(name)/sizeof(name[0]), pFile);
    
    fseek(pFile, 0, SEEK_END);
    fileSize = ftell(pFile);
    
    rewind(pFile);
    buffer = (char*) malloc(sizeof(char)*fileSize);
    
    if(buffer == NULL){
       perror ("Error Occured");
       printf ("Error Code: %d\n", errno);
              
       exit(2);
    }
  
  dataInFile = fread(buffer, 1, fileSize, pFile);
  if(dataInFile != fileSize){
  
    perror ("Error Occured");
    printf ("Error Code: %d\n", errno);
           
    exit(3);
   }
 
   printf("%s\n", buffer);
   printf("\n");
   
   fclose(pFile);
   free(buffer);
 
  return 0;
}