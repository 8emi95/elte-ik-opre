#include <stdio.h>
#include <sys/types.h>

int main() {
	pid_t gy1, gy2;
	
	gy1 = fork();
	
	if(gy1) // ha nem nulla...tehát a szülõ
	{
		gy2 = fork();
		if(gy2)
			printf("Szülõ:\n- 1. gyerek: %i\n -2. gyerek: %i\n", gy1, gy2);
		else
			printf("2. gyerek: %i\n 2.gyerek szülõje: %i\n", getpid(), gettppid());
	}
	else
		printf("1. gyerek: %i\n 1.gyerek szülõje: %i\n", getpid(), gettppid());
	
	
}