
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/signal.h>

#include <signal.h>

#include <errno.h>

static int my_parent;
void chld_sig_handler(int sig)
{
    int k;
    printf("Sheisse, mein Vater hat mich morte geworden!\n");

    kill(my_parent, SIGKILL);
}


int main(int argc, char *argv[]) {
    pid_t ch = fork();
    if (ch > 0) {
		printf("VATER: Guten Tag! Ich bin der Vater! Mein Process ID ist %d\n", getpid());
		printf("VATER: Ich warte fuer mein Kind!\n");
		sleep(5);
		printf("VATER: Ich werde mein Kind KILLen\n");
		kill(ch, 15);
		waitpid(ch, 0, 0);

		printf("VATER: Tschuss!\n");
		} 
	else if (ch == 0) {
		my_parent = getppid();
		signal(15, chld_sig_handler);

		printf("KIND: Ahoy, Ich bin das Kind! Mein Process ID ist %d\n",
		getpid());
		printf("KIND: Bitte Warten einen wenigen Minuten\n");
		sleep(10);
		printf("KIND: Tschuss!\n");
		} else {
		printf("Bang! %s\n", strerror(errno));
		}
    return 0;
}


