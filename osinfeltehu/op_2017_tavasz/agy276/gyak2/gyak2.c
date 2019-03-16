#include <stdlib.h>
#include <stdio.h>

int length(char *str){
  char *start;
  start = str;
  //int l = 0;
  while(*str++);
  return --str - start;
}

int isDifferent(char *str1, char*str2){ 
  int k = 0;
  while(*str1 || *str2){
    if(*str1 == *str2){
     k++;
    }
  str1++;
  str2++;
  }
  if(k == length(str1)){
    return 1; 
  }else{
    return 0;
  }
}

int main(){
  char str[] = "Almafa a berekben";
  printf("A string: %s , a  hossz: %i \n", str, length(str));
  
  char str1[] = "baba";
  char str2[] = "asztal";
  char str3[] = "baba";
  printf("A string: %s és a string: %s megegyezik?: %i \n", str1, str2, isDifferent(str1, str2));
  printf("A string: %s, és a string: %S megegyezik?: %i", str1, str3, isDifferent(str1, str3));

}

