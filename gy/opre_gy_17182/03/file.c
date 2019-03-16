// make a copy of file given in argv[1] to file given in argv[2]

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>     // open, creat
#include <sys/types.h> // open
#include <sys/stat.h>
#include <errno.h>     // perror, errno

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        perror("You have to use program with two arguments, the file names copy_from copy_to");
        exit(1);
    }
    int f, g;

    f = open(argv[1], O_RDONLY);

    printf("%i\n", f);

    // printf("%i", O_RDONLY);

    if (f < 0)
    {
        perror("Error at opening the file\n");
        exit(1);
    }

    g = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR); // | bitenkenti vagy-olas
    if (g < 0)
    {
        perror("Error at opening the file\n");
        exit(1);
    }

    printf("%i\n", g);

    char c;
    while (read(f, &c, sizeof(c)))
    {
        if (write(g, &c, sizeof(c)) != sizeof(c))
        {
            perror("There is a mistake in writing\n");
            exit(1);
        }
    }

    close(f);
    close(g);
    return 0;
}