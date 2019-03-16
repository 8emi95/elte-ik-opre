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

struct task{
    char task[50];
    char name[50];
    int id;
};

int main(int argc, char* argv[]){
    int pipefd[2];
    pid_t pid;
    char* argv_name = argv[1];
    char sz[100];
    if (pipe(pipefd) == -1) 
	{
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if (pid == -1) 
	{
        perror("Fork hiba");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        sleep(3);
        close(pipefd[1]);
        //struct task child_task[1];

        //read(pipefd[0], &child_task[0], sizeof(struct task));
        read(pipefd[0], &sz, sizeof(sz));
        //printf("Feladat: %s\nNév: %s\nSorszám: %i\n", child_task[0].task, child_task[0].name, child_task[0].id);
        printf("Gyerek olvasta uzenet: %d",sz);
        close(pipefd[0]);
    }
    else
    {
        close(pipefd[0]);
        /*struct task parent_to_child_task[1];
        printf("Adja meg a feladatot: ");
        scanf("%s", parent_to_child_task[0].task);
        printf("Adja meg a versenyző nevét: ");
        scanf("%s", parent_to_child_task[0].name);
        printf("Adja meg a versenyző sorszámát: ");
        scanf("%i", parent_to_child_task[0].id);*/
        //int x[4] = {25, 10, 15, 5};
        char name[50];
        printf("Adja meg a feladatot: ");
        scanf("%s", name);
        printf("Name: %s\n", name);
        //write(pipefd[1], &parent_to_child_task[0], sizeof(struct task));
        write(pipefd[1], name, sizeof(name));
        close(pipefd[1]);
        fflush(NULL);
        wait(NULL);
    }
    exit(EXIT_SUCCESS);
    //printf("%s\n", name);
}