#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int file; 
	int i; 
	char c;
	file = open("events.txt", O_RDONLY);

	//i = read(file, &c, sizeof(char));
	printf("Ez a program");
   	while(i = read(file, &c, sizeof(char))) {
   		//printf("belépek");
   		printf("%c\t%i\n",c, i);
  	}
  	printf("vegeztem");
  	close(file);
  	return 0;
}