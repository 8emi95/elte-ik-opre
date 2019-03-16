//make a copy of file given in argv[1] to file given in argv[2]
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>

struct Person
{
	char name[21];
	int date;
};

int main(int argc,char** argv){

 if (argc!=2){perror("Wrong number of params");exit(1);}
 int reader, writer;

 reader=open(argv[1],O_RDONLY);

 if (reader<0){ perror("Error at opening the file\n");exit(1);}

 writer=open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);

 if (writer<0){ perror("Error at opening the file\n");exit(1);}

 struct Person p;
 strcpy(p.name, "Gipsz Jakab");
 p.date = 1990;

 write(writer, &p, sizeof(p));
 read (reader, &p, sizeof(p));

 printf("Name: %s \nDate: %d\n", p.name, p.date);


 close(writer);
 close(reader);
 return 0;
}
