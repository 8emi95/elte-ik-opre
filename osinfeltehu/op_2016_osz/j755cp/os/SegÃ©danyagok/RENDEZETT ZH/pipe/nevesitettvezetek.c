#include <signal.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h> 

//a 2-vel egyutt kell használni
//nem enged írni amíg nincs két vége
//ezt & jellel a végén kell indítani (háttérben) és utána a 2.-at rendesen
int main()
{
	mkfifo("fifo1", S_IRUSR | S_IWUSR); //0666 is lehetne 2. paraméter
	int f = open("fifo1", O_RDONLY);
	char uzenet[25];
	read (f, uzenet, 25);
	printf(uzenet);
	close(f);
	unlink("fifo1"); //így szünteti meg a vezetéket
	
	return 0;
}
