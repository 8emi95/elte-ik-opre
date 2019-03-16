#include <stdio.h>
#include <stdlib.h>

 
char* SpaceBegone(char* c){
	
	int i = 0;
	while(c[i] != '\0' && c[i] == ' ')
		i++;

	return c+i;
};


int main(int argn, char**args){
	char* str;
	int i;
	for(i= 1;i<argn;i++){
		str = SpaceBegone(args[i]);
		printf("og: -%s-\n",args[1]);
		printf("new str: -%s-\n",str);
	}	
	return 0;
};