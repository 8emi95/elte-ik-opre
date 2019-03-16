#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	char buffer[10]= {0};
	int i=0;
	int j;
	i=read(0,buffer,10);
	printf("%i : %s\n",i,buffer);
	for (j = 0; j < 10; j++ ) {
     		printf("Element[%d] = %d\n", j, buffer[j] );
	}
	int f=open("data.txt",O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
	write(f,buffer,10);
	char buffer2[5];
	lseek(f,0,0);
	i=read(f,buffer2,5);
	printf("%i : %s\n",i,buffer2);
	if(buffer2[0]=='\n') printf("miafasz");
	close(f);
}
