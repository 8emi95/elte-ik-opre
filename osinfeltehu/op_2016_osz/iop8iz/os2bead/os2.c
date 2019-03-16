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
                close(fd[0]);
                struct feladat fc[1];

                write(fd[1], fc[0], sizeof(struct feladat));
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
        else
        {
                close(fd[1]);

                struct feladat f[1];
                f[0].feladat_neve = "asd";
                f[0].versenyzo_neve = nev;
                f[0].sorszam = 1;

                nbytes = read(fd[0], f[0], sizeof(struct feladat));
                printf("Versenyző neve: %s", nev);
                printf("\n");
                exit(EXIT_SUCCESS);
        }
        
        return(0);
}