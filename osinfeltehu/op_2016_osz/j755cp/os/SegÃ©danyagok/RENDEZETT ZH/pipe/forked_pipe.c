
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
    int fd[2];
    if (pipe(fd)) {
	perror("Crash!");
	exit(0);
    }

    ch = fork();
    if (ch > 0) {
	int k;
	char buf[255];
	bzero(buf, 0);
	printf("VATER: Guten Tag! Ich bin der Vater! Mein Process ID ist 
%d\n", getpid());
	printf("VATER: Ich wurde ein wort von mein Kind gelesen!\n");
	waitpid(ch, 0, 0);
	k = read(fd[0], buf, 255);
	if (k > 0) {
	    buf[k] = 0;
	    printf("VATER: Mein Kind schriebt: '%s'\n", buf);
	    printf("VATER: Danke Sehr Mein Kind!\n");
	}
	printf("VATER: Tschuss!\n");
    } else if (ch == 0) {
	const char *einwort="Hi Vater! Ich bin Dein Sohn!";
	write(fd[1], einwort, strlen(einwort));
	sleep(1);
	printf("KIND: Tschuss!\n");
    } else {
	printf("Bang! %s\n", strerror(errno));
    }
    return 0;
}
