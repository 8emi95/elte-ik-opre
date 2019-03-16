#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define InputFile "backup.txt"

int main(int argc, char *argv[])
{
    /*
    printf(argv[0]);
  	printf("\n");
    printf(argv[1]);
    printf("\n");
*/

    int fd[2];
    char id[2];
    char buff[300];
    char names[10][50];
    char result[10][50];
    int Pipe = pipe(fd);
    if (Pipe < 0)
    {
        perror("Pipe(fd) error!");
        exit(-1);
    }

    pid_t child = fork();
    if (child < 0)
    {
        perror("fork() error!");
        exit(-1);
    }

    /* parent process */
    if (0 != child)
    {
        int i = 0;
        FILE *f;
        strcpy(id , "2");
        f = fopen(id, "r");
        if (f == NULL)
        {
            perror("File opening error\n");
            exit(1);
        }
        char line[50];
        while (fgets(line, sizeof(line), f))
        {
            strcpy(names[i], line);
            fgets(line, sizeof(line), f);
            fgets(line, sizeof(line), f);
            ++i;
        }
        /* printf("main send --> %d=getpid() -- %d=child\n", getpid(), child); */

        write(fd[1], names, sizeof(names));
       
wait(NULL);
        read(fd[0], result, sizeof(result));
        
        
    }
    /* child process */
    else
    {
        /* printf("child send -> %d=getpid() -- %d=getppid()\n", getpid(), getppid()); */

        read(fd[0], result, sizeof(result));
        printf("A gyermekfolyamat megkapta a kovetkezo rendezveny azonositojat:\n");
        int j = 0;
        for(; j < strlen(result[10]) ; j++){
            printf("Parent %d: %s \n", j, result[j]);
        }
        //strcpy(result, "Valami");
        char str[20];
        strcpy(str, "Rendezveny: ");
        write(fd[1], str, sizeof(str)+1);
        /*****************************************************/
    }

    return 0;
}
