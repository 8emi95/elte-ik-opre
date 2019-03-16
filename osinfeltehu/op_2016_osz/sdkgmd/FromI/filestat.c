#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc,char* argv[])
{
    int fd;
    struct stat s;

    if(argc!=2)
    {
	printf("Hasznalat: filestat <filename>\n");
	return 1;
    }

    fd = open(argv[1],O_RDONLY);
    if(fd==-1)
    {
	printf("File megnyitasi hiba!\n");
	return 2;
    }

    if(fstat(fd,&s))
    {
	printf("File stat lekerdezes sikertelen.\n");
	close(fd);
	return 3;
    }

    printf("File nev: %s\n", argv[1]);
    printf("Sorozat szam: %08X\n", s.st_ino);
    printf("Linkek szama a file-ra: %d\n", s.st_nlink);
    printf("File user id: %d\n", s.st_uid);
    printf("File group id: %d\n", s.st_gid);
    printf("File meret: %u\n", s.st_size);
    printf("File tipusok: %s,%s,%s,%s\n",
	s.st_mode&S_IFIFO ? "fifo" : "-",
	s.st_mode&S_IFREG ? "reg" : "-",
	s.st_mode&S_IFDIR ? "dir" : "file",
	s.st_mode&S_IFLNK ? "lnk" : "-");
    printf("Hozzaferesi modok: %08X\n", s.st_mode);

    close(fd);

    return 0;
}
