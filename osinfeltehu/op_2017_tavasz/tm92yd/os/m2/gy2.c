#include<stdio.h>
#include<sys/types.h>

int main() {
/*
	pid_t pid;
	pid = fork();
	printf("pid=%i getpid=%i getppid=%i\n", pid,  getpid(), getppid());
*/
	//szűlőnek még 1 gyerek folyamat
	pid_t gy1, gy2;
	gy1 = fork();

	if(gy1) {
		gy2 = fork();
		if(gy2) {
			printf("Szülő: \n ----1. gyerek %i \n ----2. gyerek %i \n", gy1, gy2);
		} else {
			printf("2. gyerek %i \n 2. gyerek szülője %i \n", getpid(), getppid());
		}
	} else {
		printf("1. gyerek %i \n 1. gyerek szülője %i \n", getpid(), getppid());
	}
}
