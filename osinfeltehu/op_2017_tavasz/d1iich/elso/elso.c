#include <stdio.h>

int main(int argc,char ** argv) 
{
 char a[] = "harjra vasas!";
 int hossz= length(a);
 printf("%d", hossz);
 return 0;
}

int length(char* str){
char *ptr = str;
int n=0;
while(ptr!='\0'){
n=n+1;
*ptr=*(ptr+1);
}

return n;
}
