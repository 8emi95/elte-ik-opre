#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<errno.h>

int main()
{
	// pid_t en = getpid();
	pid_t child = fork();
	//if(child == -1) { perror("hiba"); exit(1); }
	if(child > 0)
	{
		printf("szulo\n");
		int status 
		waitpid(child, &status, 0); // gyerek bevarasa: 
		// status: a gyerek státusza, amit az oprendszer ad, mikor leáll a gyerek (0 ha oké, ha lelövik akkor annak a kódja, stb)
		// 0: tényleg várjon a gyerekre
	} else {
		printf("gyerek\n");
	}
	printf("Hello %i %i\n", getpid(), getppid());
	return 0;
}