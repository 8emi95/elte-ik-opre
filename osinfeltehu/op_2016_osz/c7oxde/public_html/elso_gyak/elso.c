#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int lenght(char * str){
 int l=0;
 while(*str++!=0)l++;
 return l;
}

int equal(char* str1,char* str2){
 for(int i=0;i<lenght(str1);i++){
  if(str1[i]!=str2[i]){
   return false;
  }
 }
 return true;
}

int main()
{
 char str1[]="Hello World!\0";
 char str2[]="!!!";
 str1[1]=='A';
 str1[1]=0;
 return 0;
 if(!equal(str1,str2))printf("Nemegyenlo");
}