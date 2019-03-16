#include <stdio.h>
#include <stdlib.h>

 
char* SpaceBegone(char* c){
	char* str = 0;
	
	int i = 0;
	while(c[i] != '\0' && c[i] == ' ')
		i++;
	
	if(c[i] != '\0'){
		int j = i+1;
		while(c[j] != 0)
			j++;
		str = (char*) malloc(j-i+1);
		int z = 0;
		for(;z < j-i+1; z++)
			str[z] = c[i+z];	
	}
	return str;
};
 


int main(int argn, char**args){
	char* str;
	int i= 1;
	for(;i<argn;i++){
		str= SpaceBegone(args[i]);
		printf("og: -%s-\n",args[1]);
		printf("new str: -%s-\n",str);
	}	
	return 0;
};