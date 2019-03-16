#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main(int argc, char** argv)
{
	char t1[4];
	char t2[4];
	
	int i;
	
		fgets(t2,4,stdin);

	//for (i=0; i < 4; i++) {
		scanf("%s", t1 );
	//}
	
	printf("t1:\n");
	printf("%s\n", t1 );
	
	printf("t2:\n");
	printf("%s\n", t2 );

	return 0;
}