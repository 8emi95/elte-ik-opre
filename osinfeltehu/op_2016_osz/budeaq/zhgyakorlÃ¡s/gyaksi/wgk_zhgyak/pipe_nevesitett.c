#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

struct Message {
	char name[100];
	int age;
};

int main() {
	int status;
	
	int fd;
	int fid = mkfifo("fifo.ftc", S_IRUSR|S_IWUSR);
	
	
	if(fid == -1) {
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
		fd = open("fifo.ftc", O_WRONLY); // Megnyitja
		
		struct Message m1 = {"Akos", 16};
		struct Message m2 = {"Krisz", 21};
		
		write(fd, &m1, sizeof(m1));
		write(fd, &m2, sizeof(m2));
		
		close(fd); // Lecsatlakozik
		
		waitpid(child, &status, 0);
		unlink("fifo.ftc"); // Törli
	} else {
		// GYEREK
		fd = open("fifo.ftc", O_RDONLY); // Megnyitja
		
		struct Message m1;
		struct Message m2;
		
		read(fd, &m1, sizeof(m1));
		printf("A szoveg: %s\nAge: %d\n", m1.name, m1.age);
		read(fd, &m2, sizeof(m2));
		printf("A szoveg: %s\nAge: %d\n", m2.name, m2.age);
		
		close(fd); // Lecsatlakozik
		exit(0);
	}
	
	return 0;
}