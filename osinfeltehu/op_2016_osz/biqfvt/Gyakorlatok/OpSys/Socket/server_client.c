#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(void)
{
    int sv[2]; /* the pair of socket descriptors */
    char buf[50]; /* for data exchange between processes */

//    if (socketpair(AF_UNIX, SOCK_DGRAM, 0, sv) == -1) {
      if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
      //not implemented for AF_INET
        perror("socketpair");
        exit(1);
    }

    if (!fork()) {  /* child */
        read(sv[1], buf, 50);
        printf("child: read '%s'\n", buf);

/*        
        buf = toupper(buf);  // make it uppercase 
        write(sv[1], &buf, 1);
        printf("child: sent '%c'\n", buf);
*/
    } else { /* parent */
        write(sv[0], "Hello world", 12);
        printf("parent: wrote \n");
     /*
        read(sv[0], &buf, 1);
        printf("parent: read '%c'\n", buf);
        */
        wait(NULL); /* wait for child to die */
    }

    return 0;
}