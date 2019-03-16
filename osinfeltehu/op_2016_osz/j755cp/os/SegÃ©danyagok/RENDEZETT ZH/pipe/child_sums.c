
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/signal.h>

#include <signal.h>

#include <errno.h>

static int my_parent;

int main(int argc, char *argv[]) {
    pid_t ch;
    int fd1[2];
    int fd2[2];
    if (pipe(fd1)) {
	perror("Crash!");
	exit(0);
    }
    if (pipe(fd2)) {
	perror("Crash!");
	exit(0);
    }

    ch = fork();
    if (ch > 0) {
	int k, m, n;
	char buf[255];
	bzero(buf, 0);
	printf("VATER: Guten Tag! Ich bin der Vater! Mein Process ID ist 
%d\n", getpid());
	m = 3;
	n = 2;
	write(fd1[1], &m, sizeof(m));
	write(fd2[1], &n, sizeof(n));
	printf("VATER: Ich wurde ein wort von mein Kind gelesen!\n");
	waitpid(ch, 0, 0);
	k = read(fd2[0], &n, sizeof(n));
	if (k > 0) {
	    buf[k] = 0;
	    printf("VATER: Mein Kind schriebt: '%d'\n", n);
	    printf("VATER: Danke Sehr Mein Kind!\n");
	}
	printf("VATER: Tschuss!\n");
    } else if (ch == 0) {
	int k1, k2, sum;
	read(fd1[0], &k1, sizeof(k1));
	read(fd2[0], &k2, sizeof(k2));
	sum = k1 + k2;
	printf("Kind: %d und %d ist %d!\n", k1, k2, sum);
	write(fd2[1], &sum, sizeof(sum));
	sleep(1);
	printf("KIND: Tschuss!\n");
    } else {
	printf("Bang! %s\n", strerror(errno));
    }
    return 0;
}
