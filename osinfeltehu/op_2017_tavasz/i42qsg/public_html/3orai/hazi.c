#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>


struct TEST {
    char first;
    short second;
    char third;
} myTest;


int main( int argc, const char* argv[] ) {
    FILE *tf;



    tf = fopen("Test.txt", "r");
    fread(&myTest, 1, 4, tf);
    fclose(tf);
    printf("First: %c\r\n", myTest.first);
    printf("Second: %u\r\n", myTest.second);
    printf("Third: %c\r\n", myTest.third);
}


