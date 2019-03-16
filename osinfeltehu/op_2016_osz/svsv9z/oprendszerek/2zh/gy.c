#include <stdio.h>


int main( int argc, const char* argv[] )
{
	printf( "\nHello World\n\n" );
    printf( "Argumentumok: %d\n", argc );

    srand(time(NULL));
    int ids[300];
    int num;
    sscanf (argv[1],"%i",&num);

    printf( "Argumentum: %i\n", num );

    int i = 0;
    for(; i < num; i++) {
        ids[i] = rand() % 10000;
        printf("id: %d", ids[i]);
    }

    pid_t child1 = fork();
    pid_t child2 = fork();

	if 
	



}