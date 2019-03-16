#include <stdio.h>
#include <stdlib.h>

int length(char *str){
 char *start=str;
 while(*str++);
 return str-start-1;
}

int equal(char *str1,char *str2){
while(*str1){
if(*str1++!=*str2++)return 0;
}
return *str2==0?1:0;
}


int main(){
char st1[]="asdfg";
char st2[]="asdfg0";
printf("%i and %i and %i",equal(st1,st2),equal(st2,st1), equal(st1,st1));
return 0;
}
