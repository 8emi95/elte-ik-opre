#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
//strlen,strcmp,strcat,strcpy and more ....

//there is no string type - instead of it You have to use char array
int length(char * str)
{
 int l=0;
 while (*str++)l++; //*str points to the next character, if it is 0, then we reached the end of the string 
                    //0 means false
 return l;
}

int main()
{
               
  char str1[]="Hello world"; //constant string - character array
  
  char str2[80]; //80 bytes long character array
  strcpy(str2,"Hello again"); //copy some value into it
  
  char* str3; //a character pointer
  str3=(char*)malloc(80*sizeof(char)); //memory allocation for 80 characters - there is no "new" keyword
  strcpy(str3,"Hello, hello, hello"); //adding value to the "string"
                                      //strcat - concatenation

  printf("The content of variables str1, str2,str3: %s, %s, %s\n",str1,str2,str3); 

  printf("\n--------------------------\n");
  printf("str3 is %s, we can split it into 3 pieces by delimiter \" \"\n",str3);
  char * tmp = strtok (str3," ");
  while (tmp != NULL){
      printf ("%s\n",tmp);
      tmp = strtok (NULL, " ");
  }
  printf("\n--------------------------\n");   
  printf("str2 is %s\n",str2);
  str2[5]=0;
  printf("overwriting the 6th character with 0 (str2[5]=0), str2 shows the value %s \n0 means the end of the strings\n",str2);
  printf("We can create an own length function searching the ending zero \n");
  printf("The length of str2 %s is %i\n",str2,length(str2));
  printf("\n--------------------------\n");
  
  char str4[]="Hello";
  char str5[]="Hello";
  printf("str4 is %s, str5 is %s\n",str4,str5);
  printf("Checking equality with ==,");
  if (str4==str5){
    printf("they are equals \n");
  }else{
    printf("they are not equal (W H Y? == checks the equality of pointers! \n");
  }  
  printf("Checking equality with strcmp(),");
  
  if (!strcmp(str4,str5)){ //strcmp gives back 0 if they are equal, but 0 means false!!
                           //the same: if (strcmp(str4,str5)==0)
    printf("they are equals \n");
  }else{
    printf("they are not equal  \n");
  } 
   
  return 0;
}
