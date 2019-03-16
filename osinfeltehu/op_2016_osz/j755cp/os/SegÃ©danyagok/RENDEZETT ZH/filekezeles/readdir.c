#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

int main (int argc, char *argv[])
{
    DIR *d;
    struct dirent *dirp;

    if ((d = opendir(".")) == NULL) {
	perror("opendir");
	exit(1);
    }

    for (dirp = readdir(d); dirp != NULL; dirp = readdir(d)) {
	if (strcmp(".", dirp->d_name) == 0 ||
	    strcmp("..", dirp->d_name) == 0) {
	    continue;
	}

	printf("%s\n", dirp->d_name);
    }

    closedir(d);
}
