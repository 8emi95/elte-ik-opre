#include <stdio.h>
#include <stdlib.h>

int main(){
  
  char buffer[1000];
  
  FILE *dFile;
  
  dFile = fopen("randomWords.txt", "r+");
  // a+ - append; r+ - read; w+ - Create enw file
  
  if(!dFile){
    printf("Error Couldn't Write to File\n");
    return 1;
  }
  /*
  //read
  while(fscanf(pFile, "%s", buffer) == 1){
    puts(buffer);
  
  }
 //another way
 /* while(fgets(buffer, 1000, pFile) != NULL){
  
  printf("%s", buffer);
  
  }
  
 /* for(int i = 0; i<10 ; i++){
  
    rndNumber = rand() % 100;
    fprintf(pFile, "%d\n", rndNumber);
  }*/
  
  fputs("Messing with strings", dFile);
  
  // SEEK_SET: start from the beginning of the file.
  // SSEK_CUR: Move based off of current position.
  // SSEK_END: move based off of starting at the end.
  
  fseek(dFile, 12, SEEK_SET); 
  
  fputs(" Files", dFile);
  
  fseek(dFile, 0, SEEK_SET);
  
  while(fgets(buffer, 1000, dFile) != NULL){
  
    printf("%s", buffer);
    
  }
    
  printf("Success\n");
  
  if(fclose(dFile) != 0){
    printf("Error: File not closed\n");
  }
  
  return 0;
}