#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 

int main() 
{
 
 char str1[80],str2[80],str3[80]; //80 byte long char arrays 
 
 /* 
 printf("Please, type a word - reading with fgets:\n");
 fgets(str1,80,stdin); 
 //it checks the length of reading - maximum 79 character plus 0 
 printf("The word was %s, which length is  %i bytes long \n(W H Y is it longer with 1 byte???  \\n is included into the variable)\n",str1,(int)strlen(str1));
 //\n will be included too, so the length of the word is longer! 
// str1[strlen(str1)-1]=0; //replace \n with 0 character
 */
 printf("\n------------------------\n"); 
 printf("Please, type a word - reading with scanf\n");
 scanf("%s",str2);
 printf("The word was %s, which length is %i bytes long \n(it is good, but  \\n remains in the buffer)\n",str2,(int)strlen(str2));


//--\n");

 printf("Please, type a word! - reading with fgets, but it does not wait for You\n");
 fgets(str3,80,stdin);
 printf("The word became %s, which length is %i (W H Y ????? - the remaining  \\n caused the problem )\n",str3,(int)strlen(str3));
 
 return 0;
}
