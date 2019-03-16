#include <stdio.h>
#include <stdlib.h>
int main(){
  char str[]="12";
  int i=atoi(str); //conversion  from string to integer 
  printf("the string value of variable str is %s, converted into integer in variable i is %i\n",str,i);
  char str2[80];
  sprintf(str2,"%i",i); //conversion from integer to string
  printf("the integer value of i is %i, the conversion of it in str2 string variable is %s\n",i,str2);  
  return 0;
}