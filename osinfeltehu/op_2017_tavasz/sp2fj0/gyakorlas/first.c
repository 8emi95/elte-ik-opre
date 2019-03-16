#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>


int length(char* str ){
	int i = 0;
	while(*str++ != 0)
		i++;
	return i;
} 

void* swap(char* str, char* x, char* y){
	char* begin = (char*)str;
	while(*begin){
		if(*begin == *y){
			*begin = *x;
		}
	*begin++;
	}
}

struct coords{
	char x[5];
	char y[5];
};
	
int main(){
	char str[]="asdf";
	printf("the length of the string is: %i \n ", length(str) );
	char str2[]="hello mamut itt a bunko";
	swap(str2,"m", "a"); 
	printf("a kicserelt szoveg: %s \n", str2);

	int f = open("coords.txt",
		O_RDWR|O_CREAT, S_IRUSR);
	struct coords c;
	printf("coords merete: %i \n", sizeof(c));

	strcpy(c.x, "12345");
	strcpy(c.y, "67890");
	write(f,&c, sizeof(c));
	close(f);
	int fRead = open("coords.txt",O_RDONLY, S_IRUSR );
	char cRead;
	while(read(fRead, &cRead, 1)){
		printf("%c \n", cRead);
	}
	close(fRead);
	return 0;
}
