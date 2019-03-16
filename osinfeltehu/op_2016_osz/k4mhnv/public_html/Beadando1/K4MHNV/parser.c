#include <stdlib.h>  
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>

int getFileContent(const char* fname, char* content[256]){
    FILE *file = fopen(fname, "r");
    char line[256];
    int db = 0;

    while(fgets(line, sizeof(line), file)){
        content[db] = line;
        db++;
    }
    fclose(file);
    return db;
}

void printContent(){
    FILE *file = fopen("registered.txt", "r");
    char line[256];

    printf("*****Jelentkezok listaja*****\n\n");

    while(fgets(line, sizeof(line), file)){
       printf("%s", line);
    }
    printf("\n\n");
    fclose(file);
}