//két folyamat, egyik fájlba írja argv[1]-t másik beolvassa és kiírja képernyõre
//nem mûködik jól, de talán két open megoldaná, csõci

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 
#include <fcntl.h> //open,creat
#include <sys/types.h> //open

int main(int argc, char** argv)
{
	if(argc > 1)
	{
		/*struct flock lock_data;
		lock_data.l_whence=SEEK_SET;
		lock_data.l_start=0;
		lock_data.l_len=0;
		int rc=0;*/
		int handle = open("./folyamatok.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
		pid_t child = fork();
		
		if(child < 0)
		{
			printf("Hiba\n");
		}
		else if(child > 0)
		{
			int status = 0;
			waitpid(child, &status, 0);
			char* input = (char*)malloc(80*sizeof(char));
			read(handle, input, sizeof(input));
			close(handle);
			printf("beolvasva: %s \n", input);
		}
		else
		{
			write(handle, &argv[1], sizeof(argv[1]));
			printf("kiirva: %s \n", argv[1]);
			char* input = (char*)malloc(80*sizeof(char));
			read(handle, input, sizeof(input));
			printf("beolvasva: %s \n", input);
			//close(handle);
		}
		
	}
	else
	{
		printf("Hiba\n");
	}
	
	return 0;
}