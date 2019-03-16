#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <time.h>

#include<sys/sem.h>
#include<semaphore.h>

#include <sys/wait.h> //waitpids  
#include <string.h>
#include <errno.h>

#include <sys/ipc.h> 
#include <sys/msg.h> 

#include <mqueue.h>

void sighandler(int signum) {
	printf("Megerkezett!\n");
}

void sighandlerForChild(int signum) {
	printf("Megkapva Child!\n");
}

void endhandler(int signum) {
	printf("Vege!\n");
}

int main(int argc, char *argv[])
{
    int fd[2];
    int val = 0;

    // create pipe descriptors
    pipe(fd);

    // fork() returns 0 for child process, child-pid for parent process.
    if (fork() != 0)
    {
        //PARENTPROCESS
	    pid_t child2 = fork(); 

        if (child2 == 0) {
            printf("CHILD2 LETREJOTT");
        }
        else {
            // parent: writing only, so close read-descriptor.
            signal()
            pause();
            pause();
            close(fd[0]);

            // send the value on the write-descriptor.
            val = 100;
            write(fd[1], &val, sizeof(val));
            printf("Parent(%d) send value: %d\n", getpid(), val);

            // close the write descriptor
            close(fd[1]);
        }
    }
    else
    {   // child: 1
        kill()

        printf("CHILD1 LETREJOTT");
        close(fd[1]);

        // now read the data (will block)
        read(fd[0], &val, sizeof(val));
        printf("Child(%d) received value: %d\n", getpid(), val);

        // close the read-descriptor
        close(fd[0]);
    }
    return 0;
}
