// encoding: western (iso 8859-1)

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h> // stat
#include <unistd.h>   // stat
#include <time.h>     // ctime
#include <string.h>   // strlen, strcmp, strcat, strcpy and more ....
#include <pwd.h>      // password

// Szoveg meretet nezi.
int length(char *str)
{
    int l = 0;
    // Addig megy a ciklus, amig meg nem talalja a szoveg vegen talalhato null-t.
    while (*str++ != 0)
        l++;
    return l;
}

int main(int argc, char **argv)
{

    // Vegigmegyunk a bemeneti argumentumokon, majd kiirjuk oket.
    // int i;
    // printf("Number of command line arguments are: %i\n", argc);
    // for (i = 0; i < argc; i++)
    // {
    //     printf("%i. argument is %s\n", i, argv[i]);
    // }

    // struct stat st;
    // stat(argv[0], &st); // fstat(file descriptor, stat structure)
    // time_t t = st.st_mtime;
    // printf("The last modification was: %s\n", ctime(&t)); // convert time_t to string

    // User id, name kiiratasa
    // struct stat st;
    // stat(argv[0], &st);
    // printf("The user id: %i\n", st.st_uid);
    // struct passwd *pwd;
    // pwd = getpwuid(st.st_uid);
    // printf("The user name: %s\n", pwd->pw_name);

    // Szoveg, es annak meretenek kiiratasa
    char str1[] = "Hello world";
    printf("The content of variable str1: \'%s\'\n", str1);
    printf("The length of variable str1 \'%s\'\n  %i (with length()), %i (with strlen())\n", str1, length(str1), strlen(str1));

    // Szoveg masolasa strcpy fuggvennyel
    char *str2;
    str2 = (char *)malloc(80 * sizeof(char));

    strcpy(str2, "New content by using strcpy");
    printf("The new content of variable str2: \'%s\'\n", str2);

    free(str2);

    return 0;
}