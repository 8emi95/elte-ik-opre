//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>

struct Student {
	char name[128];
	unsigned int grade;
};

int main(int argc,char** argv) {
	struct Student student;
	int out = open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	
	strcpy(student.name, "Teszt");
	student.grade = 5;
	
	if (out < 0) {
		perror("Error at opening the file\n");
		exit(1);
	}
	
	write(out, &student, sizeof(student));
	
	close(out);
	
	return 0;
}