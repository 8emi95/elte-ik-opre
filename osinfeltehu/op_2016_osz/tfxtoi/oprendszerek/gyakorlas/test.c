#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <time.h>

struct Visitor{
	char* event;
	char* name;
	char* emailAddress;
	time_t date;
};

int main(int argc, char** argv)
{
	int numberOfVisitor = 1;
	struct Visitor* visitors = malloc(numberOfVisitor * sizeof(struct Visitor));
	if(visitors == NULL)
	{
		perror("Allocating memory for Visitors error\n");
		exit(1);
	}
	/*struct Visitor visitor;*/

	int mem = 50;
	
	visitors->event=(char*)malloc(mem * sizeof(char));
	if(visitors->event == NULL)
	{
                perror("Allocating memory for Event error\n");
		free(visitors);
                exit(1);
        }
	visitors->name=(char*)malloc(mem * sizeof(char));
	if(visitors->name == NULL)
	{
		free(visitors);
                perror("Allocating memory for Name error\n");
                exit(1);
        }
	visitors->emailAddress=(char*)malloc(mem * sizeof(char));
	if(visitors->emailAddress == NULL)
	{
		free(visitors);
                perror("Allocating memory for Email error\n");
                exit(1);
        }
	
	printf("Mely esemenyre akar feliratkozni?:");
	fgets(visitors->event,mem,stdin);
        printf("Kerem adja meg a nevet:");
        fgets(visitors->name,mem,stdin);
	printf("MI az email cime?:");
        fgets(visitors->emailAddress,mem,stdin);
	time((&visitors->date));
	printf("\n");
	

	printf("Esemeny:%s\n",visitors->event);
	printf("Nev:%s\n",visitors->name);
	printf("Email:%s\n",visitors->emailAddress);
	printf("A regisztracio napja:%s\n",ctime(&(visitors->date)));

	char* serialised=malloc(4*mem*sizeof(char));
	strncpy(serialised,visitors->event,strlen(visitors->event)-1);
	strcat(serialised,";");
	strncat(serialised,visitors->name,strlen(visitors->name)-1);
	strcat(serialised,";");
	strncat(serialised,visitors->emailAddress,strlen(visitors->emailAddress)-1);
	strcat(serialised,";");
	strncat(serialised,ctime(&(visitors->date)),strlen(ctime(&(visitors->date)))-1);
	strcat(serialised,";\n");

	printf("Concatenated string:%s\n",serialised);
	

	int visitorsFile;
	visitorsFile = open("visitorsFile",O_WRONLY|O_CREAT|O_APPEND,S_IRUSR|S_IWUSR);
	if(visitorsFile<0)
	{
		perror("Error at opening the output file!\n");
		exit(1);
	}

	if(write(visitorsFile,&serialised,sizeof(serialised))!=sizeof(serialised))
	{
		perror("There is a mistake in writing!\n");
		exit(1);
	}
	close(visitorsFile);
	free(serialised);
	free(visitors->event);
	free(visitors->name);
	free(visitors->emailAddress);
	free(visitors);
	
	return 0;
}
