#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

int main (int argc, char *argv[])
{
    DIR *d;
    struct dirent *dirp;

    if (argc > 1) {
	d = opendir(argv[1]);
    } else {
	d = opendir(".");
    }
    
    if (d == NULL) {
	perror("opendir");
	exit(1);
    }

    for (dirp = readdir(d); dirp != NULL; dirp = readdir(d)) {
	// if (strcmp(".", dirp->d_name) == 0 ||
	//    strcmp("..", dirp->d_name) == 0) {
	//    continue;
	// }

	printf("%s [", dirp->d_name);
	switch(dirp->d_type) {
	    case DT_REG:
		printf("regular");
		break;
	    case DT_DIR:
		printf("directory");
		break;
	    case DT_LNK:
		printf("soft link");
		break;
	    case DT_CHR:
		printf("block dev");
		break;
	    case DT_BLK:
		printf("char dev");
		break;
	    case DT_SOCK:
		printf("socket");
		break;
	    case DT_FIFO:
		printf("fifo");
		break;
	    default:
		printf("%d", dirp->d_type);
		break;
	}
	printf("]\n");
    }

    closedir(d);
}
