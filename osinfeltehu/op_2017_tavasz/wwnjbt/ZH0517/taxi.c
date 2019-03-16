#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <unistd.h> 
#include <wait.h>
#define CHILDREN_COUNT 1

void handler(int signum) {}

int main(int argc, char** argv) {
	pid_t customer[CHILDREN_COUNT], driver[CHILDREN_COUNT];
	int pid, i;
	int pipefd[2];
	if (pipe(pipefd)) {
		perror("pipehiba\n");
		return 1;
	}
	
	signal(SIGUSR1, handler);
	
	for (i = 0; i < CHILDREN_COUNT; i++) {
		pid = fork();
		if (pid < 0) {
			perror("fork hiba\n");
			return 1;
		}
		else if (pid == 0) {
			char buffer[100];
			sprintf(buffer, argv[i+1]);
			close(pipefd[0]);
			write(pipefd[1], buffer, sizeof(buffer));
			printf("[CHILD](%d) Elkuldtem a lakcimet.\n", getpid());
			kill(getppid(), SIGUSR1);
			break;
		}
		else {
			customer[i] = pid;
		}
	}
	
	if (pid > 0) {

		int pid, i;
		for (i = 0; i < CHILDREN_COUNT; ++i) {
			pause();
			printf("[PARENT](%d) Bejelentkezett egy gyerek.\n", getpid());
			char buffer[100];
			close(pipefd[1]);
			read(pipefd[0], buffer, sizeof(buffer));
			printf("A kapott lakcim: %s\n", buffer);
			
			pid = fork();
			
			if (pid < 0) {
				perror("fork hiba\n");
				exit(1);
			}
			else if (pid == 0) {
				printf("[TAXI](%d) A %d utas taxisa vagyok!\n", getpid(), customer[i]);
				break;
			}
			else {
				driver[i] = pid;
			}
		}
		
	}
	

}













