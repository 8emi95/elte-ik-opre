#include <stdio.h>
#include <stdlib.h>

int main(int argc,char ** argv){
	int i;
	for(i = 0 ; i < *argv[2] - '0' ; i++ ){
		printf("%s\n",argv[1]);
	}
	return 0;
}