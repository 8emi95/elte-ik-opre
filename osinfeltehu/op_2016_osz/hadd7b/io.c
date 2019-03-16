#include <unistd.h>
#include <sys/stat.h> 
#include <fcntl.h>

#define LEN(x) (sizeof(x))

int sys(const char *cmd)
{
	return
	execl("/bin/bash", "bash", "-c", cmd, (char*)0); 
}

int main(int argc, char** argv)
{

//	int fd = open(argv[1], O_RDWR|O_TRUNC);
//	write(fd, "truncated\n", LEN("truncated\n"));
	if(sys("ls"))
	{
		sys("./io.o");
	}

	return 0;
}
