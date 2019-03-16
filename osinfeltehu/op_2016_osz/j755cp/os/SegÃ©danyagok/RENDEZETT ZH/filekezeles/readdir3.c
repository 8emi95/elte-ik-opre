#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

void rd(char *currd);

int main (int argc, char *argv[])
{
    if (argc > 1) {
	rd(argv[1]);
    } else {
	rd(".");
    }
}

void rd(char *currd) {
    DIR *d;
    struct dirent *dirp;
    char pbuf[255];
    
    d = opendir(currd);
    if (d == NULL)
	return;

    printf(">>> DIR: %s\n\n", currd);

    for (dirp = readdir(d); dirp != NULL; dirp = readdir(d)) {
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

	// rec
	if (dirp->d_type == DT_DIR &&
	    (strcmp(dirp->d_name, ".") &&
	     strcmp(dirp->d_name, "..") ) ) {
	    sprintf(pbuf, "%s/%s", currd, dirp->d_name);
	    rd(pbuf);
	}
    }

    closedir(d);
}
