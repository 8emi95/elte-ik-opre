
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int calculatel(char word[]){

  int i;
  int c=0;
  
  while(word[i]!='\0'){
    i++;
    c++;
  }
  
  return c;

}

char* function5(char *word){

  char* p;
  p=word;
  
  while(*p!=0){
  
    if(*p==' '){
    p++;
    }
    else
      return p;
  
  }

}

int main(int argc, char** argv){

  char word[80];
  
  scanf("%s",word);
  
 // int length=calculatel(word);
  
  char* res;
  res=function5(word);
  
  printf("%s\n", res);
 // int j;
 // int i;
 // j=atoi(word);
 // for(i=0; i<j; i++){
 //   printf("hello\n");
 // }
  
  return 0;

}