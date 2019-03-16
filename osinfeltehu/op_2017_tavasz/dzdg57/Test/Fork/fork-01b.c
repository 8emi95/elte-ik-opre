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
		
		// mi t�rt�nik ha m�gis printf-et haszn�lunk a write helyett... (helyes(write-os) megold�s a fork-01.c-ben)
		printf(buf);
		
	}
}