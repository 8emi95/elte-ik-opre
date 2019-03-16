#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>


int main(int argc, char** argv)
{
	int filedesc = open("testfile.txt", O_WRONLY | O_APPEND);

	write(filedesc,"This will be output to testfile.txt\n");

	if(strcmp(argv[1],"add") == 0) 
	{
		addPeopleToEvent(argc, argv);	
	}
	else if(argv[1] == "update") 
	{
		updatePeopleInEvent(argc, argv);
	}
	else if(argv[1] == "delete")
	{
		deletePeopleFromEvent(argc, argv);
	}
 	else if(argv[1] == "add_event")
	{
		addEvent(argc, argv);
	}


	return 0;
}

void addPeopleToEvent(int argc, char** argv)
{
	if(argc != 6)
	{
		printf("Résztvevő hozzáadásánál, szükségesek:  az esemény neve, és a résztvevő adatai is.");
	}
}

void updatePeopleInEvent(int argc, char** argv)
{

}

void deletePeopleFromEvent(int argc, char** argv)
{

}

void addEvent(int argc, char** argv)
{
	
}
 



