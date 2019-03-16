#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

//char** means an array of character arrays = array of strings
int main(int argc, char** argv) {
    int i;
    printf("Number of command line arguments are: %i\n", argc);
    
    for(i=0; i<argc; i++) {
        printf("%i. argument is %s\n", i, argv[i]);
    }
    return 0;
}