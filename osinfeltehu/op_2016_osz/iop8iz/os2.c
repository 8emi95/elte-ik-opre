#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h> //ctime
#include <unistd.h>  //fork, pipe
#include <sys/wait.h> //waitpid
#include <sys/time.h>
#include <signal.h>
#include <wait.h>
#include <dirent.h>
#include <linux/limits.h>

struct feladat{
    char feladat_neve[50];
    char versenyzo_neve[50];
    int sorszam;
};

int main(int argc, char* argv[])
{
        int     fd[2], nbytes;
        pid_t   childpid;
        char*   versenyzo_neve = argv[1];
        char    nev[80];

        pipe(fd);
        
        if((childpid = fork()) == -1)
        {
                perror("fork");
                exit(1);
        }

        if(childpid != 0)
        {
            close(fd[1]);

                nbytes = read(fd[0], nev, sizeof(nev));
                printf("Versenyző neve: %s", nev);
                printf("\n");
                fflush(NULL);
                wait(NULL);
               
        }
        else
        {
                 sleep(1);
                close(fd[0]);

                write(fd[1], versenyzo_neve, sizeof(versenyzo_neve));
                srand(time(NULL));
                int r=rand()%10;
                if(r > 5)
                {
                    printf("A feladat sikeres!\n");
                } else
                {
                    printf("A feladat sikertelen!\n");
                }
                exit(EXIT_SUCCESS);
        }
        
        return(0);
}