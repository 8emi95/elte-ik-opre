#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno

int main(int argc, char** argv){
	int handle = open("./vimTest.txt", O_RDONLY);
	char c;
	read(handle, &c, sizeof(c));
	printf("%c\n", c);
	close(handle);
	return 0;
}
