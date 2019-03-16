#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv){

if(argc > 1){
    char* str = argv[1];

    while(*str++ == ' ');

    printf("%s\n", --str);
}

    return 0;
}