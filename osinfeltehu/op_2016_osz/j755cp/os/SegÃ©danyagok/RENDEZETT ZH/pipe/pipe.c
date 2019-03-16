#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>

const char *to_send = "Hello pipe world!";

int main(int argc, char *argv[]) {
    int fd[2];
    char buf[255];
    
    if (!pipe(fd)) {
	//Write first
	write(fd[1], to_send, strlen(to_send));
	bzero(buf, 255);
	read(fd[0], buf, 255);
	fprintf(stderr, "read %s\n", buf);
    } else {
	perror("Cannot create pipe");
	exit(0);	
    }

    return 0;
}     