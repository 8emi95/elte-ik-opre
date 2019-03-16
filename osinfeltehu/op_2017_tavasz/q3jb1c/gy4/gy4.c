#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

void pecset(){
	kill(getppid(), SIGUSR1);
	printf("\npecsetelo\n");
}

void elnok(){
	printf("elnok\n");
}

void ellenorzo(){
	printf("ellenorzo\n");
}

volatile szamlalo = 0;
void handler(int sn){
	szamlalo++;
}

int main(int argc, char* argv[]){
	signal(SIGUSR1, handler);
	int* fd;
	if (argc != 2){
		perror("1 arg\n");
		exit(1);
	}
	
	if (pipe(fd) < 0){
		perror("hibas a cso\n");
		exit(1);
	}

	pid_t ellen = fork();
	if (ellen > 0){
		pid_t pecsetelo = fork();
		if (pecsetelo > 0){
			while (szamlalo != 2){
				usleep(100);
				elnok();
				wait(NULL);
				wait(NULL);
			}
		} else {
			pecset();
		}
	} else {
		ellenorzo();
	}
	return 0;
}
