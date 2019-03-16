#include <stdio.h>
#include <stdlib.h>

int main(){
	
	char *str1;
	char* str2;
	
   /* Initial memory allocation */
   str1 = (char *) malloc(15);
   str1 = "tutorialspoint";
   
   str2 = str1;
   //printf("String = %s,  Address = %u\n", str1, str1);
   
   free(str1);
   free(str2);
	
	return 0;
}