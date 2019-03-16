#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(void){

  FILE *pFile;
  
  size_t dataInFile;
      
   long fileSize;
        
   pFile = fopen("randomnumber.bin", "rb+");
             
   if(pFile==NULL){
     perror ("Error Occured");
     printf ("Error Code: %d\n", errno);
   
     printf("File bieng created \n\n");
   
     pFile = fopen("randomnumber.bin", "wb+");
                          
     if(pFile==NULL){
       perror ("Error Occured");
       printf ("Error Code: %d\n", errno);
                                  
       exit(1);
      }
    }                    
    
    int randNum[20];
    
    for(int i = 0; i<20; i++){
    
      randNum[i] = rand() % 100;
      printf("Number %d is %d \n", i, randNum[i]);
      
    }
    
    fwrite(randNum, sizeof(int), 20, pFile);
    
    long randomIndexNumber;
    int numberAtIndex;
    
    printf("Which Random Numeber do you want ? ");
    scanf("%d", &randomIndexNumber);
    
    fseek(pFile, 0, SEEK_SET);
    fread(&numberAtIndex, sizeof(int), 1, pFile);
    
    printf("The random Numebr at Index %d is %d\n",
         randomIndexNumber, numberAtIndex);
                                                     
    fclose(pFile);                                                          
    return 0;
}
                                                                                                                                             