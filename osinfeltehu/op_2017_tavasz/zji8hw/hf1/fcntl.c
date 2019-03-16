#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

void write_to_file(int file, struct flock *lock, char *text)
{
    int i, j, rc;
    for (j = 0; j < 25; j++)
    {
        lock->l_pid = getpid();
        lock->l_type = F_WRLCK;
        rc = fcntl(file, F_SETLKW, lock);

        if (rc != -1)
        {
            for (i = 0; i < strlen(text); i++)
            {
                write(file, &text[i], 1);
                usleep(20);
            }
            write(file, "\n", 1);
            lock->l_type = F_UNLCK;
            fcntl(file, F_SETLKW, lock);
        }
    }
}

int main(int argc, char **argv)
{
    char text1[] = " Parent Parent Parent Parent Parent Parent Parent Parent Parent Parent ";
    char text2[] = " Child Child Child Child Child Child Child Child Child Child Child Child ";

    struct flock lock_data;
    lock_data.l_whence = SEEK_SET;
    lock_data.l_start = 0;
    lock_data.l_len = 0;
    
    int f = open("data.txt", O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);

    pid_t child = fork();
    if (child < 0)
    {
        perror("Error");
        exit(1);
    }
    else if (child > 0)
    {
        write_to_file(f, &lock_data, text1);
        int status;
        waitpid(child, &status, 0);
    }
    else
    {
        write_to_file(f, &lock_data, text2);
    }

    close(f);
    return 0;
}