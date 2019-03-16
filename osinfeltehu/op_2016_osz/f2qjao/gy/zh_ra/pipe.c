#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>

struct Message {
	char name[100];
	int age;
};

int main() {
	int status;
	
	int pipefd[2];
	
	
	if(pipe(pipefd) == -1) {
		printf("HIBA");
		exit(2);
	}
	
	int child = fork();
	
	if(fork < 0) {
		printf("HIBA");
		exit(1);
	}
	
	if(child > 0) {
		// SZÜLÕ
		close(pipefd[0]);
		struct Message m1 = {"Akos", 16};
		write(pipefd[1], &m1, sizeof(m1));
		struct Message m2 = {"Krisz", 21};
		write(pipefd[1], &m2, sizeof(m2));
		close(pipefd[1]);
		fflush(NULL);
		waitpid(child, &status, 0);
	} else {
		// GYEREK
		close(pipefd[1]);
		struct Message m1;
		struct Message m2;
		read(pipefd[0], &m1, sizeof(m1));
		printf("A szoveg: %s\nAge: %d\n", m1.name, m1.age);
		read(pipefd[0], &m2, sizeof(m2));
		printf("A szoveg: %s\nAge: %d\n", m2.name, m2.age);
		close(pipefd[0]);
		exit(0);
	}
	
	return 0;
}