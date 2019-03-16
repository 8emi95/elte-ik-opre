#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define MAX_COUNT 	200
#define BUF_SIZE 	100

void main(void) {
	pid_t 	pid;
	int 	i;
	char 	buf[BUF_SIZE];
	
	fork();
	pid = getpid();
	for (i=1; i <= MAX_COUNT; i++) {
		sprintf(buf, "This line is from pid %d, value = %d\n", pid, i);
		
		// write paraméterei:
		// 1.:
		// The file descriptor of where to write the output. You can either use a file descriptor obtained from the open system call,
		// or you can use 0, 1, or 2, to refer to standard input, standard output, or standard error, respectively.
		// 2.:
		// A null terminated character string of the content to write.
		// 3.: 
		// The number of bytes to write. If smaller than the provided buffer, the output is truncated.
		// visszatérési érték: ssize_t
		// Returns the number of bytes that were written. If value is negative, then the system call returned an error.
		write(1, buf, strlen(buf));
		
		// What is the reason of using write rather than printf? It is because printf() is "buffered," meaning printf()
		// will group the output of a process together. While buffering the output for the parent process, the child may
		// also use printf to print out some information, which will also be buffered. As a result, since the output will
		// not be send to screen immediately, you may not get the right order of the expected result. Worse, the output from
		// the two processes may be mixed in strange ways. To overcome this problem, you may consider to use the "unbuffered" write.
		
	}
}