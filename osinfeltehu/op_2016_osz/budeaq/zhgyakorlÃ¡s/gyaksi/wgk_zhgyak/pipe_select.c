#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>

struct Message {
	char name[100];
	int age;
};

int main() {
	int pipe_1[2];
	int pipe_2[2];
	
	if(pipe(pipe_1) == -1) {
		printf("HIBA");
		exit(2);
	}
	
	if(pipe(pipe_2) == -1) {
		printf("HIBA");
		exit(2);
	}
	
	
	int status;
	
	int child_1 = fork();
	int child_2 = fork();
	
	if(child_1 < 0) {
		exit(1);
	}
	
	if(child_2 < 0) {
		exit(1);
	}
	
	if(child_1 == 0 && child_2 != 0) {
		// Gyerek 1
		close(pipe_1[0]);
		sleep(2);
		struct Message m = {"Akos", 16};
		write(pipe_1[1], &m, sizeof(m));
		fflush(NULL);
		
		close(pipe_1[1]);
		
		exit(0);
	}
	
	if(child_2 == 0 && child_1 != 0) {
		// Gyerek 2
		close(pipe_2[0]);
		
		struct Message m = {"Krisz", 21};
		write(pipe_2[1], &m, sizeof(m));
		fflush(NULL);
		
		close(pipe_2[1]);
		
		exit(0);
	}
	
	if(child_1 > 0 && child_2 > 0) {
		close(pipe_1[1]);
		close(pipe_2[1]);
		
		struct timeval timer;
		timer.tv_sec=5;
		timer.tv_usec=0;
		
		fd_set set;
		
		
		// 1 csatornán próbálok olvasni
			FD_ZERO(&set);
			FD_SET(pipe_1[0], &set);
			
			struct Message m1;
			struct Message m2;
			
			int result = select(FD_SETSIZE, &set, NULL,NULL, &timer);
			
			if(result < 0) {
				// ERROR
				exit(1);
			}
		
			if(result == 0) {
				// Lejárt az idõzítõ
				printf("Az idozito lejart!\n");
			} else {
				// Sikerült kiválasztani
				printf("Sikerult olvasni\n");
				
				read(pipe_1[0], &m1, sizeof(m1));
				printf("A szoveg: %s\nAge: %d\n", m1.name, m1.age);
			}
		
		
		close(pipe_1[0]);
		close(pipe_2[0]);
	}
	
	
	return 0;
}